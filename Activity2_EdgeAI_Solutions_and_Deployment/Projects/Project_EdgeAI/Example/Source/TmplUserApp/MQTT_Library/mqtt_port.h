/**
 * @file    mqtt_port.h
 * @brief   Platform Abstraction Layer (PAL) for MQTT Library
 *
 * This file defines the hardware abstraction interface that must be
 * implemented for each target MCU platform. The core MQTT logic calls
 * only the functions defined in this interface, making the library
 * fully platform-independent.
 *
 * To port to a new MCU:
 *   1. Include this header
 *   2. Implement all functions in MqttPort_Interface
 *   3. Call MqttPort_Init() with your implementation before using MQTT
 */

#ifndef MQTT_PORT_H
#define MQTT_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* ---- Platform-agnostic status codes ------------------------------------ */

typedef enum {
    MQTT_PORT_OK       = 0x00U,
    MQTT_PORT_ERROR    = 0x01U,
    MQTT_PORT_BUSY     = 0x02U,
    MQTT_PORT_TIMEOUT  = 0x03U
} MqttPort_Status;

/* ---- GPIO pin identifiers (logical) ------------------------------------ */

typedef enum {
    MQTT_GPIO_LED_CONNECTED = 0,
    MQTT_GPIO_LED_TX,
    MQTT_GPIO_LED_RX,
    MQTT_GPIO_PIN_MAX
} MqttPort_GpioPin;

typedef enum {
    MQTT_GPIO_LOW  = 0,
    MQTT_GPIO_HIGH = 1
} MqttPort_GpioState;

/* ---- Hardware abstraction interface ------------------------------------ */

typedef struct {
    /**
     * @brief  Transmit data over UART (blocking/polling).
     * @param  data    Pointer to data buffer
     * @param  len     Number of bytes to transmit
     * @param  timeout Timeout in milliseconds
     * @return MQTT_PORT_OK on success
     */
    MqttPort_Status (*uart_transmit)(const uint8_t *data, uint16_t len, uint32_t timeout);

    /**
     * @brief  Receive data over UART (blocking/polling).
     * @param  buffer  Pointer to receive buffer
     * @param  len     Number of bytes to receive
     * @param  timeout Timeout in milliseconds
     * @return MQTT_PORT_OK on success, MQTT_PORT_TIMEOUT on timeout
     */
    MqttPort_Status (*uart_receive)(uint8_t *buffer, uint16_t len, uint32_t timeout);

    /**
     * @brief  Start receiving data over UART using interrupt.
     * @param  buffer  Pointer to receive buffer
     * @param  len     Number of bytes to receive
     * @return MQTT_PORT_OK on success
     */
    MqttPort_Status (*uart_receive_it)(uint8_t *buffer, uint16_t len);

    /**
     * @brief  Abort an ongoing interrupt-based UART receive.
     * @return MQTT_PORT_OK on success
     */
    MqttPort_Status (*uart_abort_receive_it)(void);

    /**
     * @brief  Check if UART TX is ready (not busy).
     * @return 1 if TX is ready, 0 if TX is busy
     */
    uint8_t (*uart_is_tx_ready)(void);

    /**
     * @brief  Check if UART RX has data pending.
     * @return 1 if RX data available, 0 otherwise
     */
    uint8_t (*uart_is_rx_busy)(void);

    /**
     * @brief  Blocking delay.
     * @param  ms  Delay duration in milliseconds
     */
    void (*delay_ms)(uint32_t ms);

    /**
     * @brief  Get current system tick in milliseconds.
     * @return Tick count in ms (wraps around)
     */
    uint32_t (*get_tick_ms)(void);

    /**
     * @brief  Start a periodic timer with interrupt (1-second tick).
     * @return MQTT_PORT_OK on success
     */
    MqttPort_Status (*timer_start)(void);

    /**
     * @brief  Stop the periodic timer.
     * @return MQTT_PORT_OK on success
     */
    MqttPort_Status (*timer_stop)(void);

    /**
     * @brief  Write a GPIO pin (for LED indicators, etc.).
     * @param  pin    Logical GPIO pin identifier
     * @param  state  MQTT_GPIO_HIGH or MQTT_GPIO_LOW
     */
    void (*gpio_write)(MqttPort_GpioPin pin, MqttPort_GpioState state);

} MqttPort_Interface;

/* ---- Port initialization ----------------------------------------------- */

/**
 * @brief  Register the platform-specific implementation.
 *         Must be called before any MQTT core function.
 * @param  port  Pointer to a populated MqttPort_Interface struct.
 *               The struct must remain valid for the lifetime of the library.
 */
void MqttPort_Init(const MqttPort_Interface *port);

/**
 * @brief  Get the currently registered port interface.
 * @return Pointer to the active interface, or NULL if not initialized.
 */
const MqttPort_Interface* MqttPort_Get(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_PORT_H */
