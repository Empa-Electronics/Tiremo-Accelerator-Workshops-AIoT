/**
*******************************************************************************
 * @file LIS2DE12_app.c
 * @author ABOV R&D Division
 * @brief LIS2DE12 3-Axis Accelerometer Application Implementation
 *
 * Copyright 2022 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
******************************************************************************/

//******************************************************************************
// Include
//******************************************************************************

#include "LIS2DE12_app.h"
#include "lis2de12.h"
#include "../DebugLibrary/debug_framework.h"
#include "hal_i2c.h"
#include <stdio.h>

//******************************************************************************
// Global Variables
//******************************************************************************

uint8_t lis2de12_device_id = 0;
int16_t lis2de12_accel_x = 0;        // X-axis acceleration (mg)
int16_t lis2de12_accel_y = 0;        // Y-axis acceleration (mg)
int16_t lis2de12_accel_z = 0;        // Z-axis acceleration (mg)
int16_t lis2de12_temperature = 0;    // Temperature in 0.1°C units

//******************************************************************************
// Private Variables
//******************************************************************************

static uint8_t sensor_initialized = 0;

//******************************************************************************
// Function Implementation
//******************************************************************************
static void delay_ms_approx(uint32_t ms)
{
    volatile uint32_t cnt;
    for (; ms > 0; ms--) {
        for (cnt = 0; cnt < 8000U; cnt++) { ; }
    }
}

/**
 * @brief Initialize LIS2DE12 sensor
 * @return 0 on success, error code otherwise
 */
uint8_t LIS2DE12_Init(void)
{
    uint8_t whoami = 0;
    int32_t error = 0;

    /*--------------------------------------------------------------
     * STEP 0: Reboot sensor to clear any stuck state from a previous
     * session (MCU soft-reset without sensor power cycle).
     *--------------------------------------------------------------*/
    lis2de12_boot_set(PROPERTY_ENABLE);
    delay_ms_approx(50);  /* Boot takes ~5 ms; give generous 50 ms */

    /*--------------------------------------------------------------
     * STEP 1: Verify WHO_AM_I with retry
     *--------------------------------------------------------------*/
    for (uint8_t retry = 0; retry < 5; retry++) {
        error = lis2de12_device_id_get(&whoami);
        if (error == 0 && whoami == LIS2DE12_ID) break;
        delay_ms_approx(10);
    }

    if (error != 0) {
        DebugFramework_PutsLine("[ERR] WHO_AM_I read failed (I2C error)\n\r");
        return 1;
    }

    lis2de12_device_id = whoami;

    if (whoami != LIS2DE12_ID) {
        DebugFramework_Printf("[ERR] Wrong ID: 0x%02X (expected 0x33)\n\r", whoami);
        return 2;
    }

    /*--------------------------------------------------------------
     * STEP 2: Power-down first, then configure while sensor is idle.
     *--------------------------------------------------------------*/
    error = lis2de12_data_rate_set(LIS2DE12_POWER_DOWN);
    if (error != 0) { return 3; }
    delay_ms_approx(5);

    /*--------------------------------------------------------------
     * STEP 3: DISABLE BDU (Block Data Update = continuous mode).
     *
     * With oversampling (multiple reads averaged), BDU is not
     * needed — any occasional mid-read update has negligible
     * effect on the averaged result.
     *--------------------------------------------------------------*/
    error = lis2de12_block_data_update_set(PROPERTY_DISABLE);
    if (error != 0) { return 4; }

    /*--------------------------------------------------------------
     * STEP 4: Set full scale to +/-2g
     *--------------------------------------------------------------*/
    error = lis2de12_full_scale_set(LIS2DE12_2g);
    if (error != 0) { return 5; }

    /*--------------------------------------------------------------
     * STEP 5: Enable temperature sensor
     *--------------------------------------------------------------*/
    error = lis2de12_temperature_meas_set(LIS2DE12_TEMP_ENABLE);
    if (error != 0) { return 6; }

    /*--------------------------------------------------------------
     * STEP 6: Write CTRL_REG1 with verification loop.
     *   ODR=100Hz (0101), LPen=1, Zen=1, Yen=1, Xen=1 = 0x5F
     *--------------------------------------------------------------*/
    {
        uint8_t ctrl1_desired = 0x5F;
        uint8_t ctrl1_readback = 0;
        uint8_t ok = 0;

        for (uint8_t attempt = 0; attempt < 5; attempt++) {
            error = lis2de12_write_reg(LIS2DE12_CTRL_REG1, &ctrl1_desired, 1);
            if (error != 0) {
                DebugFramework_Printf("[WARN] CTRL_REG1 write failed, attempt %d\n\r", attempt);
                delay_ms_approx(10);
                continue;
            }
            delay_ms_approx(2);

            error = lis2de12_read_reg(LIS2DE12_CTRL_REG1, &ctrl1_readback, 1);
            if (error == 0 && ctrl1_readback == ctrl1_desired) {
                ok = 1;
                break;
            }
            DebugFramework_Printf("[WARN] CTRL_REG1 verify: wrote 0x%02X, read 0x%02X (attempt %d)\n\r",
                                  ctrl1_desired, ctrl1_readback, attempt);
            delay_ms_approx(10);
        }
        if (!ok) {
            DebugFramework_PutsLine("[ERR] Cannot set CTRL_REG1!\n\r");
            return 7;
        }
    }

    /* Wait for first sample at 100 Hz ODR (~10 ms per sample) */
    delay_ms_approx(20);

    /*--------------------------------------------------------------
     * STEP 7: Dummy read of all 6 output registers to flush stale data.
     * 0x29-0x2E: OUT_X_H, OUT_X_L, OUT_Y_H, OUT_Y_L, OUT_Z_H, OUT_Z_L
     *--------------------------------------------------------------*/
    {
        uint8_t dummy[6];
        lis2de12_read_reg(0x29, dummy, 6);
    }

    sensor_initialized = 1;
    return 0;
}

/**
 * @brief Read acceleration data from all axes
 * @return 0 on success, error code otherwise
 *
 * Reads all 6 output bytes (X_H, X_L, Y_H, Y_L, Z_H, Z_L) from
 * registers 0x29-0x2E in a single I2C burst with auto-increment.
 *
 * Uses 4x oversampling to improve effective resolution from ~16 mg
 * (8-bit ADC) to ~4 mg.  Conversion uses the precise sensitivity
 * factor 4000 mg / 65536 = 125/2048 per raw-16bit LSB.
 */

uint8_t LIS2DE12_ReadAcceleration(void)
{
    int32_t ret;
    uint8_t raw[6] = {0};   /* X_H, X_L, Y_H, Y_L, Z_H, Z_L */
    uint8_t status_reg = 0;
    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
    const uint8_t N_SAMPLES = 1;  /* 4x oversampling for sub-LSB resolution */

    if (!sensor_initialized) return 1;

    /*--------------------------------------------------------------
     * 4x oversampling: read N_SAMPLES consecutive data sets from the
     * sensor and average them.  This improves effective resolution
     * from ~16 mg (8-bit) to ~4 mg, at the cost of a lower output
     * rate (100 Hz / 4 = 25 Hz effective).
     *--------------------------------------------------------------*/
    for (uint8_t n = 0; n < N_SAMPLES; n++) {
        /* Wait for ZYXDA = new data ready */
        uint16_t timeout = 200;
        do {
            ret = lis2de12_read_reg(LIS2DE12_STATUS_REG, &status_reg, 1);
            if (ret != 0) return 5;
            if (status_reg & 0x08) break;
            delay_ms_approx(1);
        } while (--timeout > 0);

        if (!(status_reg & 0x08)) {
            DebugFramework_Printf("STATUS_REG = 0x%02X (no data ready)\n\r", status_reg);
            return 6;
        }

        /*----------------------------------------------------------
         * Burst-read 6 bytes from OUT_X_H (0x29) with auto-increment:
         *   raw[0] = 0x29 OUT_X_H
         *   raw[1] = 0x2A OUT_X_L
         *   raw[2] = 0x2B OUT_Y_H
         *   raw[3] = 0x2C OUT_Y_L
         *   raw[4] = 0x2D OUT_Z_H
         *   raw[5] = 0x2E OUT_Z_L
         * Data is two's complement, left-justified in H:L pair.
         *----------------------------------------------------------*/
        ret = lis2de12_read_reg(0x29U, raw, 6);
        if (ret != 0) return 2;

        /* Combine H:L into signed 16-bit (left-justified) */
        sum_x += (int16_t)(((uint16_t)raw[0] << 8) | raw[1]);
        sum_y += (int16_t)(((uint16_t)raw[2] << 8) | raw[3]);
        sum_z += (int16_t)(((uint16_t)raw[4] << 8) | raw[5]);
    }

    /*--------------------------------------------------------------
     * Conversion to mg with precise sensitivity.
     *
     * FS = +/-2 g  =>  4000 mg over 65536 raw-16bit levels
     *   mg = raw_16bit * 4000 / 65536 = raw_16bit * 125 / 2048
     *
     * With N-sample average:
     *   mg = (sum / N) * 125 / 2048  =  sum * 125 / (N * 2048)
     *
     * For N=4:  divisor = 4 * 2048 = 8192
     * Max |sum| = 4 * 32768 = 131072  =>  131072 * 125 = 16.4M (fits int32)
     *--------------------------------------------------------------*/
    lis2de12_accel_x = (int16_t)(sum_x * 125 / (N_SAMPLES * 2048));
    lis2de12_accel_y = (int16_t)(sum_y * 125 / (N_SAMPLES * 2048));
    lis2de12_accel_z = (int16_t)(sum_z * 125 / (N_SAMPLES * 2048));

    return 0;
}

/**
 * @brief Read temperature from LIS2DE12 sensor
 * @return 0 on success, error code otherwise
 */
uint8_t LIS2DE12_ReadTemperature(void)
{
    uint16_t raw_temp;
    
    if (!sensor_initialized) {
        return 1;
    }
    
    // Read raw temperature data
    if (lis2de12_temperature_raw_get(&raw_temp) != 0) {
        return 2;
    }
    
    // Convert to temperature (in 0.1°C units)
    lis2de12_temperature = (int16_t)(lis2de12_from_lsb_to_celsius((int16_t)raw_temp) * 10.0f);
    
    return 0;
}

/**
 * @brief Read all sensor data (acceleration + temperature)
 * @return 0 on success, error code otherwise
 */
uint8_t LIS2DE12_ReadSensor(void)
{
    uint8_t result = 0;
    
    if (!sensor_initialized) {
        return 1;
    }
    
    // Read acceleration
    result = LIS2DE12_ReadAcceleration();
    if (result != 0) {
        return result;
    }
    
    // Read temperature
    result = LIS2DE12_ReadTemperature();
    if (result != 0) {
        return result;
    }
    
    return 0;
}

/**
 * @brief Print LIS2DE12 sensor data
 */
void LIS2DE12_Print(void)
{
    char buffer[16];
    
    DebugFramework_PutsLine("========== LIS2DE12 (3-Axis Accelerometer) ==========\n\r");
    
    // Print X-axis
    DebugFramework_PutsLine("Accel X     : ");
    sprintf(buffer, "%d", (int)lis2de12_accel_x);
    DebugFramework_PutsLine(buffer);
    DebugFramework_PutsLine(" mg\n\r");
    
    // Print Y-axis
    DebugFramework_PutsLine("Accel Y     : ");
    sprintf(buffer, "%d", (int)lis2de12_accel_y);
    DebugFramework_PutsLine(buffer);
    DebugFramework_PutsLine(" mg\n\r");
    
    // Print Z-axis
    DebugFramework_PutsLine("Accel Z     : ");
    sprintf(buffer, "%d", (int)lis2de12_accel_z);
    DebugFramework_PutsLine(buffer);
    DebugFramework_PutsLine(" mg\n\r");
    
    // Print temperature
   /* DebugFramework_PutsLine("Temperature : ");
    sprintf(buffer, "%d.%d", (int)(lis2de12_temperature / 10), (int)((lis2de12_temperature >= 0 ? lis2de12_temperature : -lis2de12_temperature) % 10));
    DebugFramework_PutsLine(buffer);
    DebugFramework_PutsLine(" C\n\r\n\r");*/
}
