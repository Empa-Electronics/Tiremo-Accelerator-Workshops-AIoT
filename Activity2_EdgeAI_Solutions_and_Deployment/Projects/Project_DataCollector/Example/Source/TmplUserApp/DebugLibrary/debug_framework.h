/**
 *******************************************************************************
 * @file        debug_framework.h
 * @author      Custom Debug Framework
 * @brief       Configurable UART debug framework
 *******************************************************************************/

#ifndef _DEBUG_FRAMEWORK_H_
#define _DEBUG_FRAMEWORK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "hal_uart.h"
#include "hal_pcu.h"

/* -------------------------------------------------------------------------- */
/* UART profile selection                                                      */
/* -------------------------------------------------------------------------- */
#define DEBUG_UART_PROFILE_PA2_PA3_UART7      0
#define DEBUG_UART_PROFILE_PA8_PA9_UART2      1
#define DEBUG_UART_PROFILE_PA10_PA11_UART1    2
#define DEBUG_UART_PROFILE_PB8_PB9_UART3      3
#define DEBUG_UART_PROFILE_PC0_PC1_UART0      4
#define DEBUG_UART_PROFILE_PC5_PC6_UART1      5
#define DEBUG_UART_PROFILE_PC14_PC15_UART0    6
#define DEBUG_UART_PROFILE_PD6_PD7_UART2      7
#define DEBUG_UART_PROFILE_PD8_PD9_UART12     8
#define DEBUG_UART_PROFILE_PE6_PE7_UART6      9
#define DEBUG_UART_PROFILE_PE13_PE14_UART4    10
#define DEBUG_UART_PROFILE_PF2_PF3_UART8      11
#define DEBUG_UART_PROFILE_PF4_PF5_UART5      12
#define DEBUG_UART_PROFILE_PF8_PF9_UART9      13
#define DEBUG_UART_PROFILE_PF12_PF13_UART10   14
#define DEBUG_UART_PROFILE_PG5_PG6_UART11     15
#define DEBUG_UART_PROFILE_PG8_PG9_UART3      16

#ifndef DEBUG_UART_PROFILE
#define DEBUG_UART_PROFILE DEBUG_UART_PROFILE_PC14_PC15_UART0
#endif

#if (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PA2_PA3_UART7)
#define DEBUG_UART_ID_DEFAULT         UART_ID_7
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_A
#define DEBUG_UART_RX_PIN_DEFAULT     2
#define DEBUG_UART_TX_PIN_DEFAULT     3
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PA8_PA9_UART2)
#define DEBUG_UART_ID_DEFAULT         UART_ID_2
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_A
#define DEBUG_UART_RX_PIN_DEFAULT     8
#define DEBUG_UART_TX_PIN_DEFAULT     9
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PA10_PA11_UART1)
#define DEBUG_UART_ID_DEFAULT         UART_ID_1
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_A
#define DEBUG_UART_RX_PIN_DEFAULT     10
#define DEBUG_UART_TX_PIN_DEFAULT     11
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PB8_PB9_UART3)
#define DEBUG_UART_ID_DEFAULT         UART_ID_3
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_B
#define DEBUG_UART_RX_PIN_DEFAULT     8
#define DEBUG_UART_TX_PIN_DEFAULT     9
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PC0_PC1_UART0)
#define DEBUG_UART_ID_DEFAULT         UART_ID_0
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_C
#define DEBUG_UART_RX_PIN_DEFAULT     0
#define DEBUG_UART_TX_PIN_DEFAULT     1
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_3
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_3
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PC5_PC6_UART1)
#define DEBUG_UART_ID_DEFAULT         UART_ID_1
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_C
#define DEBUG_UART_RX_PIN_DEFAULT     5
#define DEBUG_UART_TX_PIN_DEFAULT     6
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_3
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_3
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PC14_PC15_UART0)
#define DEBUG_UART_ID_DEFAULT         UART_ID_0
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_C
#define DEBUG_UART_RX_PIN_DEFAULT     14
#define DEBUG_UART_TX_PIN_DEFAULT     15
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_3
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_3
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PD6_PD7_UART2)
#define DEBUG_UART_ID_DEFAULT         UART_ID_2
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_D
#define DEBUG_UART_RX_PIN_DEFAULT     7
#define DEBUG_UART_TX_PIN_DEFAULT     6
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PD8_PD9_UART12)
#define DEBUG_UART_ID_DEFAULT         UART_ID_12
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_D
#define DEBUG_UART_RX_PIN_DEFAULT     8
#define DEBUG_UART_TX_PIN_DEFAULT     9
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PE6_PE7_UART6)
#define DEBUG_UART_ID_DEFAULT         UART_ID_6
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_E
#define DEBUG_UART_RX_PIN_DEFAULT     6
#define DEBUG_UART_TX_PIN_DEFAULT     7
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PE13_PE14_UART4)
#define DEBUG_UART_ID_DEFAULT         UART_ID_4
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_E
#define DEBUG_UART_RX_PIN_DEFAULT     14
#define DEBUG_UART_TX_PIN_DEFAULT     13
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PF2_PF3_UART8)
#define DEBUG_UART_ID_DEFAULT         UART_ID_8
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_F
#define DEBUG_UART_RX_PIN_DEFAULT     2
#define DEBUG_UART_TX_PIN_DEFAULT     3
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PF4_PF5_UART5)
#define DEBUG_UART_ID_DEFAULT         UART_ID_5
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_F
#define DEBUG_UART_RX_PIN_DEFAULT     5
#define DEBUG_UART_TX_PIN_DEFAULT     4
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PF8_PF9_UART9)
#define DEBUG_UART_ID_DEFAULT         UART_ID_9
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_F
#define DEBUG_UART_RX_PIN_DEFAULT     8
#define DEBUG_UART_TX_PIN_DEFAULT     9
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PF12_PF13_UART10)
#define DEBUG_UART_ID_DEFAULT         UART_ID_10
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_F
#define DEBUG_UART_RX_PIN_DEFAULT     12
#define DEBUG_UART_TX_PIN_DEFAULT     13
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PG5_PG6_UART11)
#define DEBUG_UART_ID_DEFAULT         UART_ID_11
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_G
#define DEBUG_UART_RX_PIN_DEFAULT     5
#define DEBUG_UART_TX_PIN_DEFAULT     6
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#elif (DEBUG_UART_PROFILE == DEBUG_UART_PROFILE_PG8_PG9_UART3)
#define DEBUG_UART_ID_DEFAULT         UART_ID_3
#define DEBUG_UART_PORT_ID_DEFAULT    PCU_ID_G
#define DEBUG_UART_RX_PIN_DEFAULT     8
#define DEBUG_UART_TX_PIN_DEFAULT     9
#define DEBUG_UART_RX_ALT_DEFAULT     PCU_ALT_1
#define DEBUG_UART_TX_ALT_DEFAULT     PCU_ALT_1
#else
#error "Invalid DEBUG_UART_PROFILE"
#endif

/* Optional overrides (if needed) */
#ifndef DEBUG_UART_ID
#define DEBUG_UART_ID DEBUG_UART_ID_DEFAULT
#endif
#ifndef DEBUG_UART_PORT_ID
#define DEBUG_UART_PORT_ID DEBUG_UART_PORT_ID_DEFAULT
#endif
#ifndef DEBUG_UART_RX_PIN
#define DEBUG_UART_RX_PIN DEBUG_UART_RX_PIN_DEFAULT
#endif
#ifndef DEBUG_UART_TX_PIN
#define DEBUG_UART_TX_PIN DEBUG_UART_TX_PIN_DEFAULT
#endif
#ifndef DEBUG_UART_RX_ALT
#define DEBUG_UART_RX_ALT DEBUG_UART_RX_ALT_DEFAULT
#endif
#ifndef DEBUG_UART_TX_ALT
#define DEBUG_UART_TX_ALT DEBUG_UART_TX_ALT_DEFAULT
#endif

#ifndef DEBUG_UART_BAUDRATE
#define DEBUG_UART_BAUDRATE 115200U
#endif

/*
 * Default clock config is MCCR+HSI so UART baud stays stable even if system
 * clock (MCLK/HCLK/PCLK) changes.
 */
#ifndef DEBUG_UART_CLK_SOURCE
#define DEBUG_UART_CLK_SOURCE UART_CLK_MCCR
#endif

#ifndef DEBUG_UART_CLK_MCCR_SOURCE
#define DEBUG_UART_CLK_MCCR_SOURCE UART_CLK_MCCR_HSI
#endif

#ifndef DEBUG_UART_CLK_MCCR_DIV
#define DEBUG_UART_CLK_MCCR_DIV 1U
#endif

bool DebugFramework_Init(void);
void DebugFramework_Uninit(void);
bool DebugFramework_Reinit(void);

void DebugFramework_PutChar(uint8_t ch);
void DebugFramework_Puts(const char *str);
void DebugFramework_PutsLine(const char *str);
void DebugFramework_PutDec(uint8_t num);
void DebugFramework_PutDec16(uint16_t num);
void DebugFramework_PutDec32(uint32_t num);
void DebugFramework_PutHex(uint8_t num);
void DebugFramework_PutHex16(uint16_t num);
void DebugFramework_PutHex32(uint32_t num);
void DebugFramework_Printf(const char *format, ...);

uint8_t DebugFramework_GetChar(void);
bool DebugFramework_IsDataAvailable(void);
bool DebugFramework_IsInitialized(void);

#ifdef __cplusplus
}
#endif

#endif /* _DEBUG_FRAMEWORK_H_ */
