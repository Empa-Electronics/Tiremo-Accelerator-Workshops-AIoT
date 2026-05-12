/**
 * @file    mqtt_port_abov.c
 * @brief   ABOV MCU (A34G43x) implementation of the MQTT port interface
 *
 * This file implements all hardware-specific operations using the
 * ABOV HAL library for A34G43x series microcontrollers.
 *
 * Usage:
 *   #include "mqtt_port.h"
 *   extern void MqttPort_ABOV_Init(void);
 *
 *   int main(void) {
 *       SystemInit();
 *       // ... peripheral init (UART, Timer, GPIO) ...
 *       MqttPort_ABOV_Init();    // Register ABOV port
 *       MQTT_Init(&config);      // Now use MQTT normally
 *   }
 *
 * Prerequisites:
 *   - UART must be initialized via HAL_UART_Init() before calling MqttPort_ABOV_Init()
 *   - GPIO pins must be configured as outputs for LED indicators
 *   - Timer must be initialized for 1-second periodic interrupt
 */

#include "mqtt_port.h"

/* ---- ABOV HAL includes ------------------------------------------------ */
#include "hal_uart.h"
#include "hal_pcu.h"
#include "hal_timer1.h"
#include "system_a34xxxx.h"

/* ---- Configuration: change these to match your board ------------------ */

/* UART instance used for ESP32 communication */
#define MQTT_UART_ID            UART_ID_2

/* Timer instance used for 1-second periodic tick */
#define MQTT_TIMER_ID           TIMER1_ID_0

/* GPIO mapping for LED indicators */
/* Format: { PCU port, PCU pin } */
typedef struct {
    PCU_ID_e     port;
    PCU_PIN_ID_e pin;
} AbovGpioMapping;

static const AbovGpioMapping gpio_map[MQTT_GPIO_PIN_MAX] = {
    [MQTT_GPIO_LED_WIFI]      = { PCU_ID_B, PCU_PIN_ID_12 },  /* LED6 - WiFi connected */
    [MQTT_GPIO_LED_CONNECTED] = { PCU_ID_B, PCU_PIN_ID_13 },  /* LED7 - MQTT connected */
    [MQTT_GPIO_LED_TX]        = { PCU_ID_B, PCU_PIN_ID_11 },  /* LED5 - TX blink */
    [MQTT_GPIO_LED_RX]        = { PCU_ID_B, PCU_PIN_ID_14 },  /* LED8 - RX blink */
};

/* ---- Internal: simple tick counter for timeout (driven by SysTick) ---- */

static volatile uint32_t s_tick_ms = 0;

/**
 * Call this from your SysTick_Handler or a 1ms timer ISR:
 *   void SysTick_Handler(void) { MqttPort_ABOV_TickIncrement(); }
 */
void MqttPort_ABOV_TickIncrement(void)
{
    s_tick_ms++;
}

/* ---- Port function implementations ----------------------------------- */

static MqttPort_Status abov_uart_transmit(const uint8_t *data, uint16_t len, uint32_t timeout)
{
    (void)timeout;

    HAL_ERR_e status = HAL_UART_Transmit(MQTT_UART_ID, (uint8_t *)data, (uint32_t)len, true);

    switch (status) {
        case HAL_ERR_OK:      return MQTT_PORT_OK;
        case HAL_ERR_BUSY:    return MQTT_PORT_BUSY;
        case HAL_ERR_TIMEOUT: return MQTT_PORT_TIMEOUT;
        default:              return MQTT_PORT_ERROR;
    }
}

static MqttPort_Status abov_uart_receive(uint8_t *buffer, uint16_t len, uint32_t timeout)
{
    /*
     * Read one byte at a time using polling mode.
     * HAL_UART_Receive for 1 byte returns quickly:
     *   - HAL_ERR_OK if a byte was received
     *   - HAL_ERR_TIMEOUT if no data available (internal byte timeout)
     * We loop until our own timeout (driven by s_tick_ms from SysTick) expires.
     * This matches ESP32 AT response timing (AT+RST needs ~2s for reboot).
     */
    uint32_t start_tick = s_tick_ms;
    uint16_t received = 0;

    while (received < len) {
        if ((s_tick_ms - start_tick) >= timeout) {
            break;
        }

        HAL_ERR_e status = HAL_UART_Receive(MQTT_UART_ID, &buffer[received], 1, true);

        if (status == HAL_ERR_OK) {
            received++;
        }
        /* HAL_ERR_TIMEOUT / HAL_ERR_BUSY = no byte yet, keep trying until our timeout */
    }

    return (received > 0) ? MQTT_PORT_OK : MQTT_PORT_TIMEOUT;
}

static MqttPort_Status abov_uart_receive_it(uint8_t *buffer, uint16_t len)
{
    /*
     * ABOV uses bEnForcePoll=false to use interrupt mode.
     * IRQ handler must be set up beforehand via HAL_UART_SetIRQ().
     */
    HAL_ERR_e status = HAL_UART_Receive(MQTT_UART_ID, buffer, (uint32_t)len, false);

    switch (status) {
        case HAL_ERR_OK:   return MQTT_PORT_OK;
        case HAL_ERR_BUSY: return MQTT_PORT_BUSY;
        default:           return MQTT_PORT_ERROR;
    }
}

static MqttPort_Status abov_uart_abort_receive_it(void)
{
    HAL_ERR_e status = HAL_UART_Abort(MQTT_UART_ID);
    return (status == HAL_ERR_OK) ? MQTT_PORT_OK : MQTT_PORT_ERROR;
}

static uint8_t abov_uart_is_tx_ready(void)
{
    bool busy = false;
    HAL_UART_GetBusyStatus(MQTT_UART_ID, &busy);
    return busy ? 0 : 1;
}

static uint8_t abov_uart_is_rx_busy(void)
{
    /*
     * ABOV HAL doesn't have a direct RXNE flag check like STM32.
     * Use line status register to check for data ready.
     */
    uint8_t lineStatus = 0;
    HAL_UART_GetLineStatus(MQTT_UART_ID, &lineStatus);
    /* Bit 0 of line status = Data Ready (DR) on most ABOV UARTs */
    return (lineStatus & 0x01) ? 1 : 0;
}

static void abov_delay_ms(uint32_t ms)
{
    SystemDelayMS(ms);
}

static uint32_t abov_get_tick_ms(void)
{
    return s_tick_ms;
}

static MqttPort_Status abov_timer_start(void)
{
    HAL_ERR_e status = HAL_TIMER1_Start(MQTT_TIMER_ID);
    return (status == HAL_ERR_OK) ? MQTT_PORT_OK : MQTT_PORT_ERROR;
}

static MqttPort_Status abov_timer_stop(void)
{
    HAL_ERR_e status = HAL_TIMER1_Stop(MQTT_TIMER_ID);
    return (status == HAL_ERR_OK) ? MQTT_PORT_OK : MQTT_PORT_ERROR;
}

static void abov_gpio_write(MqttPort_GpioPin pin, MqttPort_GpioState state)
{
    if (pin < MQTT_GPIO_PIN_MAX) {
        /* LEDs are active-low: HIGH request = pin CLEAR (on), LOW request = pin SET (off) */
        PCU_OUTPUT_BIT_e bit = (state == MQTT_GPIO_HIGH) ? PCU_OUTPUT_BIT_CLEAR : PCU_OUTPUT_BIT_SET;
        HAL_PCU_SetOutputBit(gpio_map[pin].port, gpio_map[pin].pin, bit);
    }
}

/* ---- Interface struct ------------------------------------------------- */

static const MqttPort_Interface abov_port = {
    .uart_transmit         = abov_uart_transmit,
    .uart_receive          = abov_uart_receive,
    .uart_receive_it       = abov_uart_receive_it,
    .uart_abort_receive_it = abov_uart_abort_receive_it,
    .uart_is_tx_ready      = abov_uart_is_tx_ready,
    .uart_is_rx_busy       = abov_uart_is_rx_busy,
    .delay_ms              = abov_delay_ms,
    .get_tick_ms           = abov_get_tick_ms,
    .timer_start           = abov_timer_start,
    .timer_stop            = abov_timer_stop,
    .gpio_write            = abov_gpio_write,
};

/* ---- Public init function --------------------------------------------- */

void MqttPort_ABOV_Init(void)
{
    MqttPort_Init(&abov_port);
}
