/**
*******************************************************************************
 * @file LIS2DE12_app.h
 * @author ABOV R&D Division
 * @brief LIS2DE12 3-Axis Accelerometer Application Header
 *
 * Copyright 2022 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
******************************************************************************/

#ifndef LIS2DE12_APP_H
#define LIS2DE12_APP_H

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Include
//******************************************************************************

#include <stdint.h>

//******************************************************************************
// Constant
//******************************************************************************

#define LIS2DE12_I2C_ADDRESS    0x19  // 7-bit address (SA0=1, 0x33 >> 1)
// Alternative address: 0x18 if SA0=0 (0x31 >> 1)

//******************************************************************************
// Global Variables
//******************************************************************************

extern uint8_t lis2de12_device_id;
extern int16_t lis2de12_accel_x;     // X-axis acceleration (mg)
extern int16_t lis2de12_accel_y;     // Y-axis acceleration (mg)
extern int16_t lis2de12_accel_z;     // Z-axis acceleration (mg)
extern int16_t lis2de12_temperature; // Temperature in 0.1°C units

//******************************************************************************
// Function Prototypes
//******************************************************************************

/**
 * @brief Initialize LIS2DE12 sensor
 * @return 0 on success, error code otherwise
 */
uint8_t LIS2DE12_Init(void);

/**
 * @brief Read acceleration data from all axes
 * @return 0 on success, error code otherwise
 */
uint8_t LIS2DE12_ReadAcceleration(void);

/**
 * @brief Read temperature from LIS2DE12 sensor
 * @return 0 on success, error code otherwise
 */
uint8_t LIS2DE12_ReadTemperature(void);

/**
 * @brief Read all sensor data (acceleration + temperature)
 * @return 0 on success, error code otherwise
 */
uint8_t LIS2DE12_ReadSensor(void);

/**
 * @brief Print LIS2DE12 sensor data
 */
void LIS2DE12_Print(void);

#ifdef __cplusplus
}
#endif

#endif /* LIS2DE12_APP_H */
