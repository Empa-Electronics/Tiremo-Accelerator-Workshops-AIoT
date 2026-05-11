/**
 *******************************************************************************
 * @file        prv_user_code.c
 * @author      ABOV R&D Division
 * @brief       LIS2DE12 Data Collector - continuous JSON output
 *
 * Copyright 2024 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include "abov_config.h"
#include "abov_module_config.h"
#include "DebugLibrary/debug_framework.h"
#include "hal_pcu.h"
#include "hal_i2c.h"
#include "LISDE12TR/LIS2DE12_app.h"
/* ---------- AI INCLUDE STARTS HERE --------- */
#include "tiremo_api.h"
/* ---------- AI INCLUDE PATH ENDS HERE --------- */

#define TC_SYSTICK_1MS      1000

#define I2C_SCL_PORT        PCU_ID_B
#define I2C_SCL_PIN         PCU_PIN_ID_6
#define I2C_SDA_PORT        PCU_ID_B
#define I2C_SDA_PIN         PCU_PIN_ID_7
#define I2C_ALT_FUNCTION    PCU_ALT_1

extern uint32_t SystemCoreClock;
static volatile uint32_t s_un32SysTimerVal = 0;
static int16_t data[384];
/* ---------- AI GLOBALS STARTS HERE --------- */
static const char *label;
/* ---------- AI GLOBALS ENDS HERE --------- */

void SysTick_Handler(void)
{
    if (s_un32SysTimerVal)
    {
        s_un32SysTimerVal--;
    }
}

void SYSTICK_Wait(uint32_t un32TimeMS)
{
    s_un32SysTimerVal = un32TimeMS;
    while (s_un32SysTimerVal);
}

void PRV_USER_Code(void)
{
    SysTick_Config(SystemCoreClock / TC_SYSTICK_1MS);
    HAL_PCU_SetAltMode(I2C_SCL_PORT, I2C_SCL_PIN, I2C_ALT_FUNCTION);
    HAL_PCU_SetAltMode(I2C_SDA_PORT, I2C_SDA_PIN, I2C_ALT_FUNCTION);
    DebugFramework_Init();
    LIS2DE12_Init();

    while (1)
    {

        for (int i = 0; i < 128; i++)
        {
            if (LIS2DE12_ReadAcceleration() == 0)
            {
                data[i * 3 + 0] = lis2de12_accel_x;
                data[i * 3 + 1] = lis2de12_accel_y;
                data[i * 3 + 2] = lis2de12_accel_z;
            }
            SYSTICK_Wait(8);
        }
                for (int i = 0; i < 128; i++)
        {
            DebugFramework_Printf("%d %d %d", data[i * 3 + 0], data[i * 3 + 1], data[i * 3 + 2]);
        }
        DebugFramework_Printf("\n\r");
 
        /* ---------- AI INFERENCE STARTS HERE --------- */
        int result = tiremo_ai_classify_label(data, 384, &label);
        if (result == 0 && label != NULL)
        {
            DebugFramework_Printf("Classification: %s\r\n", label);
        }
        /* ---------- AI INFERENCE ENDS HERE --------- */

    }
}
