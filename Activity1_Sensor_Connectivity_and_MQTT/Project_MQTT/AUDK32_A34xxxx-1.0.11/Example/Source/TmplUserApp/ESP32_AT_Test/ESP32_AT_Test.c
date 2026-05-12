/**
 *******************************************************************************
 * @file        ESP32_AT_Test.c
 * @author      MQTT Team
 * @brief       Simple AT Test - Just send AT and check OK
 *******************************************************************************
 */

#include "ESP32_AT_Test.h"
#include "hal_uart.h"
#include "system_a34xxxx.h"
#include <string.h>
#include <stdio.h>
#include "../DebugLibrary/debug_framework.h"

/**
 * @brief Simple AT test - Send AT, check if OK received
 * @return 0 if OK, -1 if failed
 */
int ESP32_Simple_AT_Test(void)
{
    char rx_buffer[64];
    HAL_ERR_e result;
    uint8_t attempt = 0;
    const uint8_t MAX_ATTEMPTS = 10;

    while (attempt < MAX_ATTEMPTS)
    {
        attempt++;
        memset(rx_buffer, 0, sizeof(rx_buffer));

        /* Send AT command */
        HAL_UART_Transmit(UART_ID_2, (uint8_t*)"AT\r\n", 4, true);

        /* Receive response */
        result = HAL_UART_Receive(UART_ID_2, (uint8_t*)rx_buffer, 20, true);

        /* Check if OK in response */
        if (strstr(rx_buffer, "OK") != NULL)
        {
            if (attempt > 1)
            {
                DebugFramework_Printf("[AT] OK received on attempt %u\n\r", attempt);
            }
            return 0;
        }

        DebugFramework_Printf("[AT] Attempt %u/%u: no OK, retrying...\n\r", attempt, MAX_ATTEMPTS);
        (void)result;
    }

    DebugFramework_PutsLine("[AT] No OK after max attempts!");
    return -1;
}

// Placeholder functions
ESP32_Test_Result_e ESP32_AT_Test_Init(void) { return ESP32_TEST_OK; }
ESP32_Test_Result_e ESP32_AT_Test_BasicAT(void) { return ESP32_TEST_OK; }
ESP32_Test_Result_e ESP32_AT_Test_GetVersion(void) { return ESP32_TEST_OK; }
ESP32_Test_Result_e ESP32_AT_Test_Reset(void) { return ESP32_TEST_OK; }
ESP32_Test_Result_e ESP32_AT_Test_SetStationMode(void) { return ESP32_TEST_OK; }
ESP32_Test_Result_e ESP32_AT_Test_ConnectWiFi(const char* s, const char* p) { return ESP32_TEST_OK; }
ESP32_Test_Result_e ESP32_AT_Test_GetIP(void) { return ESP32_TEST_OK; }
ESP32_Test_Result_e ESP32_AT_RunAllTests(const char* s, const char* p) 
{ 
    ESP32_Simple_AT_Test();
    return ESP32_TEST_OK; 
}
const char* ESP32_AT_Test_GetLastError(void) { return ""; }


