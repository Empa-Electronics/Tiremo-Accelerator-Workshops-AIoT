/**
 *******************************************************************************
 * @file        battery_reading.c
 * @author      Battery Reading Module
 * @brief       Simple battery voltage measurement implementation
 *
 * Copyright 2024 ABOV Semiconductor Co.,Ltd. All rights reserved.
 ******************************************************************************/

#include "battery_reading.h"
#include "../DebugLibrary/debug_framework.h"

/* Private variables */
static bool s_bInitialized = false;
static uint16_t s_un16VcoreRaw = 0;
static uint32_t s_un32AvddMv = 0;

/**
 * @brief Initialize battery reading module
 */
uint8_t BatteryReading_Init(void)
{
    /* ADC is already initialized by PRV_ADC_Init() in user_adc.c */
    /* Wait for internal VCORE reference to stabilize (at least 10ms) */
    for(volatile uint32_t i = 0; i < 500000; i++);
    
    s_bInitialized = true;
    
    return 0;
}

/**
 * @brief Read ADC channel value
 */
uint8_t BatteryReading_ReadChannel(uint8_t un8Channel, uint16_t *pun16Value)
{
    HAL_ERR_e eErr;
    ADC_SEQ_DATA_t tAdcData;
    ADC_SEQ_TRG_CFG_t tSeqCfg;
    
    if (!s_bInitialized || pun16Value == NULL)
    {
        return 1;
    }
    
    /* Configure sequence for the requested channel */
    tSeqCfg.eType = ADC_TRG_TYPE_INDEPENDENT;
    tSeqCfg.eTrgSrc = ADC_TRG_SRC_ADST;
    tSeqCfg.un8TrgNum = 0;
    tSeqCfg.utCfg.tInd.un8SeqNum = 0;
    tSeqCfg.utCfg.tInd.un8ChNum = un8Channel;
    tSeqCfg.utCfg.tInd.un8SamplingTime = 10;
    
    eErr = HAL_ADC_SetSeqConfig(BATTERY_ADC_CHANNEL, &tSeqCfg);
    if (eErr != HAL_ERR_OK)
    {
        return 2;
    }
    
    /* Start ADC conversion */
    eErr = HAL_ADC_Start(BATTERY_ADC_CHANNEL);
    if (eErr != HAL_ERR_OK)
    {
        return 3;
    }
    
    /* Wait for conversion to complete */
    for(volatile uint32_t i = 0; i < 10000; i++);
    
    /* Read ADC data */
    tAdcData.bReadDDR = false;
    eErr = HAL_ADC_GetData(BATTERY_ADC_CHANNEL, 0, &tAdcData);
    if (eErr != HAL_ERR_OK)
    {
        HAL_ADC_Stop(BATTERY_ADC_CHANNEL);
        return 4;
    }
    
    *pun16Value = tAdcData.un16Result;
    
    /* Stop ADC */
    HAL_ADC_Stop(BATTERY_ADC_CHANNEL);
    
    return 0;
}

/**
 * @brief Read MCU supply voltage (AVDD) using VCORE reference
 */
uint8_t BatteryReading_ReadSupplyVoltage(uint16_t *pun16VcoreRaw, uint32_t *pun32AvddMillivolts)
{
    uint8_t result;
    uint16_t un16VcoreRaw = 0;

    if (!s_bInitialized || pun16VcoreRaw == NULL || pun32AvddMillivolts == NULL)
    {
        return 1;
    }

    /* Read VCORE (internal 1.0V reference on channel 23) */
    result = BatteryReading_ReadChannel(BATTERY_VCORE_CH, &un16VcoreRaw);
    if (result != 0)
    {
        return result;
    }
    
    *pun16VcoreRaw = un16VcoreRaw;
    
    /* Calculate AVDD (supply voltage) using VCORE measurement */
    /* AVDD = (1.0V * 4096) / VcoreRaw */
    /* In millivolts: AVDD_mV = (1000mV * 4096) / VcoreRaw */
    if (un16VcoreRaw > 0)
    {
        *pun32AvddMillivolts = (1000UL * 4096UL) / un16VcoreRaw;
    }
    else
    {
        *pun32AvddMillivolts = 0;
        return 5;
    }

    /* Store for later use in Print */
    s_un16VcoreRaw = un16VcoreRaw;
    s_un32AvddMv = *pun32AvddMillivolts;

    return 0;
}

/**
 * @brief Print supply voltage
 */
void BatteryReading_Print(void)
{
    if (!s_bInitialized)
    {
        DebugFramework_Puts("Battery: Not initialized\n\r");
        return;
    }

    /* Read supply voltage */
    if (BatteryReading_ReadSupplyVoltage(&s_un16VcoreRaw, &s_un32AvddMv) != 0)
    {
        DebugFramework_Puts("AVDD: Read Error\n\r");
        return;
    }

    /* Print MCU supply voltage (AVDD) */
    uint32_t voltage_int = s_un32AvddMv / 1000;
    uint32_t voltage_dec = s_un32AvddMv % 1000;
    
    DebugFramework_Puts("AVDD: ");
    DebugFramework_PutDec32(voltage_int);
    DebugFramework_PutChar('.');
    if (voltage_dec < 100) DebugFramework_PutChar('0');
    if (voltage_dec < 10) DebugFramework_PutChar('0');
    DebugFramework_PutDec32(voltage_dec);
    DebugFramework_Puts("V (Vcore raw: ");
    DebugFramework_PutDec32(s_un16VcoreRaw);
    DebugFramework_Puts(")\n\r");
}
