/**
 *******************************************************************************
 * @file        battery_reading.h
 * @author      Battery Reading Module
 * @brief       Simple battery voltage measurement using ADC VCORE reference
 *
 * Copyright 2024 ABOV Semiconductor Co.,Ltd. All rights reserved.
 ******************************************************************************/

#ifndef _BATTERY_READING_H_
#define _BATTERY_READING_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "hal_adc.h"

/* ADC Configuration */
#define BATTERY_ADC_CHANNEL         ADC_ID_2
#define BATTERY_VCORE_CH            23         // VCORE channel (internal 1.0V reference)

/**
 * @brief Initialize battery reading module
 * @note ADC must be already initialized via PRV_ADC_Init()
 * @return 0 on success, non-zero on error
 */
uint8_t BatteryReading_Init(void);

/**
 * @brief Read ADC channel value
 * @param[in] un8Channel - ADC channel number
 * @param[out] pun16Value - Pointer to store ADC raw value
 * @return 0 on success, non-zero on error
 */
uint8_t BatteryReading_ReadChannel(uint8_t un8Channel, uint16_t *pun16Value);

/**
 * @brief Read MCU supply voltage (AVDD) using VCORE reference
 * @param[out] pun16VcoreRaw - VCORE raw ADC value
 * @param[out] pun32AvddMillivolts - Calculated AVDD in millivolts
 * @return 0 on success, non-zero on error
 */
uint8_t BatteryReading_ReadSupplyVoltage(uint16_t *pun16VcoreRaw, uint32_t *pun32AvddMillivolts);

/**
 * @brief Print supply voltage via debug framework
 */
void BatteryReading_Print(void);

#ifdef __cplusplus
}
#endif

#endif /* _BATTERY_READING_H_ */
