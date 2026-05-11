/**
*******************************************************************************
 * @file SHT40_app.h
 * @author ABOV R&D Division
 * @brief SHT40 Temperature and Humidity Sensor Application Header
 *
 * Copyright 2022 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
******************************************************************************/

#ifndef __SHT40_APP_H
#define __SHT40_APP_H

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Include
//******************************************************************************

#include <stdint.h>
#include "sht4x_i2c.h"
#include "sensirion_i2c_hal.h"

//******************************************************************************
// External Variables
//******************************************************************************

extern uint32_t sht40_serialnumber;
extern int32_t sht40_temperature;
extern int32_t sht40_humidity;

//******************************************************************************
// Function Prototypes
//******************************************************************************

/**
 * @brief Initialize SHT40 sensor
 * 
 * This function initializes the I2C communication and prepares the SHT40 sensor
 * for operation. Also reads the sensor serial number.
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_Init(void);

/**
 * @brief Read temperature from SHT40 sensor
 * 
 * Reads only temperature value from the sensor and updates sht40_temperature variable.
 * Temperature is in milli degrees Celsius (e.g., 25000 = 25.000°C)
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_ReadTemperature(void);

/**
 * @brief Read humidity from SHT40 sensor
 * 
 * Reads only humidity value from the sensor and updates sht40_humidity variable.
 * Humidity is in milli percent RH (e.g., 50000 = 50.000%RH)
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_ReadHumidity(void);

/**
 * @brief Read both temperature and humidity from SHT40 sensor
 * 
 * Reads both temperature and humidity values from the sensor in a single measurement.
 * Updates sht40_temperature and sht40_humidity variables.
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_ReadSensor(void);

/**
 * @brief Print SHT40 sensor data
 * 
 * Prints the current temperature and humidity values via debug interface.
 * Values are formatted and displayed in human-readable format.
 */
void SHT40_Print(void);

/**
 * @brief Reset SHT40 sensor
 * 
 * Performs a soft reset of the SHT40 sensor
 * 
 * @return 0 on success, error code otherwise
 */
uint8_t SHT40_Reset(void);

#ifdef __cplusplus
}
#endif

#endif /* __SHT40_APP_H */
