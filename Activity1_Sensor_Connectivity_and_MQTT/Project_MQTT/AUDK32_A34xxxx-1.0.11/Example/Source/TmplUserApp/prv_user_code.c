/**
 *******************************************************************************
 * @file        prv_user_code.c
 * @author      ABOV R&D Division
 * @brief       Dummy User Application Main
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
#include "hal_timer1.h"
#include "ESP32_AT_Test/ESP32_AT_Test.h"
#include "MQTT_Library/EMPA_MqttAws.h"
#include "Sensor/sensor.h"

/* ---------------------------------------------------------------
 * Feature selection:
 *   EMPA_SENSOR_PROCESS : sensor data printed to terminal
 *   EMPA_MQTT_AWS       : sensor data sent to MQTT broker (1s cycle)
 *   Both defined        : print AND send to MQTT
 * --------------------------------------------------------------- */
//#define EMPA_SENSOR_PROCESS
//#define EMPA_MQTT_AWS

extern void MqttPort_ABOV_Init(void);
extern void MqttPort_ABOV_TickIncrement(void);
extern void MP23ABS1_TimerHandler(void);

#define TC_SYSTICK_1MS      1000

/* LED Pin Definitions (active-low) */
#define LED1_PORT   PCU_ID_B
#define LED1_PIN    PCU_PIN_ID_4   // PB4
#define LED2_PORT   PCU_ID_B
#define LED2_PIN    PCU_PIN_ID_9   // PB9
#define LED3_PORT   PCU_ID_F
#define LED3_PIN    PCU_PIN_ID_7   // PF7
#define LED4_PORT   PCU_ID_B
#define LED4_PIN    PCU_PIN_ID_5   // PB5
#define LED5_PORT   PCU_ID_B
#define LED5_PIN    PCU_PIN_ID_10  // PB10 
#define LED6_PORT   PCU_ID_B
#define LED6_PIN    PCU_PIN_ID_11  // PB11 - MQTT TX blink
#define LED7_PORT   PCU_ID_B
#define LED7_PIN    PCU_PIN_ID_12  // PB12 - WiFi connected 
#define LED8_PORT   PCU_ID_B
#define LED8_PIN    PCU_PIN_ID_13  // PB13 - MQTT connected
#define LED9_PORT   PCU_ID_B
#define LED9_PIN    PCU_PIN_ID_14  // PB14
#define LED10_PORT  PCU_ID_B
#define LED10_PIN   PCU_PIN_ID_15   // PF7

/* Include HAL header files for your target modules */
#define I2C_SCL_PORT        PCU_ID_B    // PB6
#define I2C_SCL_PIN         PCU_PIN_ID_6
#define I2C_SDA_PORT        PCU_ID_B    // PB7
#define I2C_SDA_PIN         PCU_PIN_ID_7
#define I2C_ALT_FUNCTION    PCU_ALT_1

/* UART_ID_2 Configuration (ESP32-AT Module) */
#define UART2_PORT          PCU_ID_A    // PA8=RXD, PA9=TXD
#define UART2_RXD_PIN       PCU_PIN_ID_8
#define UART2_TXD_PIN       PCU_PIN_ID_9
#define UART2_ALT_FUNCTION  PCU_ALT_1

extern uint32_t SystemCoreClock;
static volatile uint32_t s_un32SysTimerVal=0;

/**********************************************************************
 * @brief		ARM System Timer Interrupt Handler.
 * @param[in]	None
 * @return	    None
 * @note        Also calls MP23ABS1_TimerHandler for microphone sampling
 **********************************************************************/
void SysTick_Handler (void)
{
    static uint32_t s_un32TickCounter = 0;
    
    if (s_un32SysTimerVal)
    {
        s_un32SysTimerVal--;
    }
    
    /* MQTT port tick (1ms) - needed for UART receive timeout */
    MqttPort_ABOV_TickIncrement();
    
    /* mqtt_timer increment (when enabled) */
    if (mqtt_timer_en) {
        mqtt_timer++;
    }
    
    /* Call MP23ABS1 timer handler every 16 ticks (for 1kHz SysTick, this gives ~62.5us per sample) */
    s_un32TickCounter++;
    if (s_un32TickCounter >= 16)
    {
        s_un32TickCounter = 0;
        MP23ABS1_TimerHandler();
    }
}

/**********************************************************************
 * @brief		Waiting by time(ms)
 * @param[in]	un32TimeMS : Milisecond time to wait.
 * @return	    None
 **********************************************************************/
void SYSTICK_Wait (uint32_t un32TimeMS)
{
    s_un32SysTimerVal = un32TimeMS;
    while(s_un32SysTimerVal);
}

/**********************************************************************
 * @brief       Turn ON specific LED
 * @param[in]   LED_PIN : LED pin ID
 * @return      None
 **********************************************************************/
void LED_ON(PCU_ID_e port, PCU_PIN_ID_e pin)
{
    HAL_PCU_SetOutputBit(port, pin, PCU_OUTPUT_BIT_CLEAR);
}

/**********************************************************************
 * @brief       Turn OFF specific LED
 * @param[in]   LED_PIN : LED pin ID
 * @return      None
 **********************************************************************/
void LED_OFF(PCU_ID_e port, PCU_PIN_ID_e pin)
{
    HAL_PCU_SetOutputBit(port, pin, PCU_OUTPUT_BIT_SET);
}

/**********************************************************************
 * @brief       Blink specific LED
 * @param[in]   LED_PIN : LED pin ID
 * @param[in]   times : Number of blinks
 * @return      None
 **********************************************************************/
void LED_Blink(PCU_ID_e port, PCU_PIN_ID_e pin, uint32_t times)
{
    for(uint32_t i = 0; i < times; i++)
    {
        LED_ON(port, pin);
        SYSTICK_Wait(100);
        LED_OFF(port, pin);
        SYSTICK_Wait(100);
    }
}

/**********************************************************************
 * @brief       Configure GPIO Alternate Functions
 * @param[in]   None
 * @return      None
 **********************************************************************/
void GPIO_Config_Alt()
{
    /* Configure all Test LEDs as GPIO output (ALT_0) */
    HAL_PCU_SetAltMode(LED1_PORT, LED1_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED2_PORT, LED2_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED3_PORT, LED3_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED4_PORT, LED4_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED5_PORT, LED5_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED6_PORT, LED6_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED7_PORT, LED7_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED8_PORT, LED8_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED9_PORT, LED9_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(LED10_PORT, LED10_PIN, PCU_ALT_0);

    /* Turn off all LEDs initially (active-low: SET = off) */
    LED_OFF(LED1_PORT,  LED1_PIN);
    LED_OFF(LED2_PORT,  LED2_PIN);
    LED_OFF(LED3_PORT,  LED3_PIN);
    LED_OFF(LED4_PORT,  LED4_PIN);
    LED_OFF(LED5_PORT,  LED5_PIN);
    LED_OFF(LED6_PORT,  LED6_PIN);
    LED_OFF(LED7_PORT,  LED7_PIN);
    LED_OFF(LED8_PORT,  LED8_PIN);
    LED_OFF(LED9_PORT,  LED9_PIN);
    LED_OFF(LED10_PORT, LED10_PIN);

    /* Configure I2C pins */
    HAL_PCU_SetAltMode(I2C_SCL_PORT, I2C_SCL_PIN, I2C_ALT_FUNCTION);
    HAL_PCU_SetAltMode(I2C_SDA_PORT, I2C_SDA_PIN, I2C_ALT_FUNCTION);

    /* Configure UART2 pins (ESP32-AT Module) */
    HAL_PCU_SetAltMode(UART2_PORT, UART2_RXD_PIN, UART2_ALT_FUNCTION);
    HAL_PCU_SetAltMode(UART2_PORT, UART2_TXD_PIN, UART2_ALT_FUNCTION);
}

/**********************************************************************
 * @brief       User Code Here
 * @param[in]   None
 * @return      None
 **********************************************************************/
void PRV_USER_Code(void)
{
    SysTick_Config(SystemCoreClock/TC_SYSTICK_1MS);
    GPIO_Config_Alt();
    
    /* 1. Debug Framework Init */
    if (DebugFramework_Init())
    {
        LED_ON(LED1_PORT, LED1_PIN);
    }
    else
    {
        LED_Blink(LED1_PORT, LED1_PIN, 5);
    }

    DebugFramework_PutsLine("\n\r\n\r");
    DebugFramework_PutsLine("##################################");
	DebugFramework_PutsLine("#                                #");
    DebugFramework_PutsLine("#         Tiremo Cortex          #");
    DebugFramework_PutsLine("#              v2.0              #");
	DebugFramework_PutsLine("#                                #");
    DebugFramework_PutsLine("##################################\n\r");
    

    /* 2. LED Hardware Test */
    Sensor_LEDTest();

    /* 3. Sensor Test - initialize and verify all sensors */
    uint8_t sensorFails = Sensor_TestAll();
    if (sensorFails > 0)
    {
        DebugFramework_Printf("[WARN] %u sensor(s) failed init\n\r", sensorFails);
    }
    SYSTICK_Wait(1000);
    /* 4. ESP32 AT Test */
#if defined(EMPA_MQTT_AWS)
    DebugFramework_PutsLine("\n\r========================================");
    DebugFramework_PutsLine("       ESP32 & MQTT CONNECTIVITY TEST       ");
    DebugFramework_PutsLine("========================================\n\r");

    DebugFramework_PutsLine("[TEST] ESP32-AT WiFi Module...");
    if (ESP32_Simple_AT_Test() == 0)
    {
        DebugFramework_PutsLine("[PASS] ESP32 OK");
    }
    else
    {
        DebugFramework_PutsLine("[FAIL] ESP32 NOT responding");
    }
#endif

    /* 5. MQTT Connect */
#if defined(EMPA_MQTT_AWS)
    DebugFramework_PutsLine("\n\r[MQTT] Connecting to broker...");
    MqttPort_ABOV_Init();
    uint8_t mqttConnected = (MQTT_ConnectBroker() == 0) ? 1 : 0;
    if (mqttConnected)
        DebugFramework_PutsLine("[MQTT] Ready to publish sensor data\n\r");
    else
        DebugFramework_PutsLine("[MQTT] Will retry each cycle\n\r");
#endif

    /* 6. Main Loop */
    uint32_t cycle = 0;
    (void)cycle;
    int mqtt_data_count = 1;
    while (1)
    {
#if defined(EMPA_SENSOR_PROCESS)
        DebugFramework_PutsLine("\n\r========================================");
        DebugFramework_Printf("   CYCLE #%lu\n\r", cycle++);
        DebugFramework_PutsLine("========================================");
        SensorData_t *pData = Sensor_ReadAndPrint();
#elif defined(EMPA_MQTT_AWS)
        SensorData_t *pData = Sensor_ReadOnly();
#endif

#if defined(EMPA_MQTT_AWS)
        if (mqttConnected)
        {
            DebugFramework_PutsLine("[MQTT] Data sending...");
            MQTT_PublishSensorData(pData);
            DebugFramework_Printf("[MQTT] Data sent! #%d\n\r", mqtt_data_count++);
        }
        else
        {
            /* Bağlantı koptu - LED6 ve LED7 söndür */
            LED_OFF(LED6_PORT, LED6_PIN);
            LED_OFF(LED7_PORT, LED7_PIN);
#if defined(EMPA_SENSOR_PROCESS)
            DebugFramework_PutsLine("[MQTT] Not connected, retrying...");
#endif
            mqttConnected = (MQTT_ConnectBroker() == 0) ? 1 : 0;
            if (mqttConnected)
            {
                MQTT_PublishSensorData(pData);
#if defined(EMPA_SENSOR_PROCESS)
                DebugFramework_PutsLine("[MQTT] Reconnected & data sent!");
#endif
            }
        }
#endif
    SYSTICK_Wait(100);
    }
}
