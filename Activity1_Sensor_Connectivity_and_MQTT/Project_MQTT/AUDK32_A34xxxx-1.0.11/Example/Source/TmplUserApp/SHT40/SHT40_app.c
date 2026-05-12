/**
*******************************************************************************
 * @file SHT40_app.c
 * @author ABOV R&D Division
 * @brief SHT40 Temperature and Humidity Sensor Application Implementation
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

#include "SHT40_app.h"
#include <stdio.h>
#include "../DebugLibrary/debug_framework.h"

//******************************************************************************
// Global Variables
//******************************************************************************

uint32_t sht40_serialnumber = 0;
int32_t sht40_temperature = 0;
int32_t sht40_humidity = 0;

//******************************************************************************
// Private Variables
//******************************************************************************

static uint8_t sensor_initialized = 0;

//******************************************************************************
// Function Implementation
//******************************************************************************

/**
 * @brief Initialize SHT40 sensor
 * 
 * This function initializes the I2C communication and prepares the SHT40 sensor
 * for operation. Also reads the sensor serial number.
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_Init(void)
{
    int8_t error = 0;
    // Try to read serial number to verify communication
    error = (int8_t)sht4x_serial_number(&sht40_serialnumber);
    
    if (error == 0) {
        sensor_initialized = 1;
    } else {
        sensor_initialized = 0;
    }
    
    return (uint8_t)error;
}

/**
 * @brief Read temperature from SHT40 sensor
 * 
 * Reads only temperature value from the sensor and updates sht40_temperature variable.
 * Temperature is in milli degrees Celsius (e.g., 25000 = 25.000°C)
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_ReadTemperature(void)
{
    int32_t temp_dummy = 0;
    
    // Check if sensor is initialized
    if (!sensor_initialized) {
        return 1;
    }
    
    // Read both values but only use temperature
    int8_t error = (int8_t)sht4x_measure_high_precision(&sht40_temperature, &temp_dummy);
    
    return (uint8_t)error;
}

/**
 * @brief Read humidity from SHT40 sensor
 * 
 * Reads only humidity value from the sensor and updates sht40_humidity variable.
 * Humidity is in milli percent RH (e.g., 50000 = 50.000%RH)
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_ReadHumidity(void)
{
    int32_t hum_dummy = 0;
    
    // Check if sensor is initialized
    if (!sensor_initialized) {
        return 1;
    }
    
    // Read both values but only use humidity
    int8_t error = (int8_t)sht4x_measure_high_precision(&hum_dummy, &sht40_humidity);
    
    return (uint8_t)error;
}

/**
 * @brief Read both temperature and humidity from SHT40 sensor
 * 
 * Reads both temperature and humidity values from the sensor in a single measurement.
 * Updates sht40_temperature and sht40_humidity variables.
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_ReadSensor(void)
{
    // Check if sensor is initialized
    if (!sensor_initialized) {
        return 1;
    }
    
    // Read both temperature and humidity
    int8_t error = (int8_t)sht4x_measure_high_precision(&sht40_temperature, &sht40_humidity);
    
    return (uint8_t)error;
}

/**
 * @brief Print SHT40 sensor data
 * 
 * Print6s the current temperature and humidity values via debug interface.
 * Values are formatted and displayed in human-readable format.
 */
void SHT40_Print(void)
{
    int32_t temp_int, temp_dec;
    int32_t hum_int, hum_dec;
    
    DebugFramework_PutsLine( "=== SHT40 Data ===\n\r" );
    
    // Calculate temperature (milli-degrees to degrees with 1 decimal)
    temp_int = sht40_temperature / 1000;
    temp_dec = ((sht40_temperature >= 0 ? sht40_temperature : -sht40_temperature) % 1000) / 100;
    
    // Calculate humidity (milli-percent to percent with 1 decimal)
    hum_int = sht40_humidity / 1000;
    hum_dec = (sht40_humidity % 1000) / 100;
    
    // Print values one by one
    DebugFramework_Puts( "Temp: " );
    DebugFramework_Printf( "%d", (int)temp_int );
    DebugFramework_Puts( "." );
    DebugFramework_Printf( "%d", (int)temp_dec );
    DebugFramework_Puts( " C\n\r" );
    
    DebugFramework_Puts( "Hum: " );
    DebugFramework_Printf( "%d", (int)hum_int );
    DebugFramework_Puts( "." );
    DebugFramework_Printf( "%d", (int)hum_dec );
    DebugFramework_Puts( " RH\n\r\n\r" );
}

/**
 * @brief Reset SHT40 sensor
 * 
 * Performs a soft reset of the SHT40 sensor
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_Reset(void)
{
    int8_t error = 0;
    
    error = (int8_t)sht4x_soft_reset();
    
    if (error == 0) {
        // Wait for sensor to reset (typical 1ms)
        sensirion_i2c_hal_sleep_usec(2000); // 2ms delay
    }
    
    return (uint8_t)error;
}
