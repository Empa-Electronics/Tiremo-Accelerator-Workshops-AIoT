/**
 * @file    mqtt_core.h
 * @brief   Platform-independent MQTT over ESP32 AT Commands - Core API
 *
 * This header contains all type definitions, configuration structures,
 * and function prototypes for the MQTT core library.
 * NO platform-specific (HAL, register, GPIO) dependencies allowed here.
 */

#ifndef MQTT_CORE_H
#define MQTT_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "mqtt_port.h"

/* ---- Configuration Defines -------------------------------------------- */

#ifndef MQTT_BROKER_ADDRESS
#define MQTT_BROKER_ADDRESS             "mqtt.example.com"
#endif

#define WIFI_FUNCS_STD_TIMEOUT          5000    /* ms */
#define WIFI_FUNCS_STD_BUFF_SIZE        90
#define MQTT_DATA_PACKET_BUFF_SIZE      600
#define MAC_ID_CHAR_SIZE                17
#define MAC_ID_UINT_SIZE                6
#define MQTT_CONN_MAX_TRY               3

#define MQTT_PACKET_SIZE_WITHOUT_DATA   (5 + MAC_ID_UINT_SIZE)
#define MQTT_CRC_DATA_SIZE              (MQTT_PACKET_SIZE_WITHOUT_DATA - 2)
#define PUB_RESP_DATA_RMNG_CHAR_COUNT   36

/* ---- Type Definitions ------------------------------------------------- */

typedef enum {
    FUNC_OK        = 0x00U,
    FUNC_RX_ERROR  = 0x01U,
    FUNC_BUSY      = 0x02U,
    FUNC_TIMEOUT   = 0x03U,
    FUNC_TX_ERROR  = 0x04U,
    FUNC_FAIL
} FUNC_StatusTypeDef;

typedef enum {
    FUNC_SUCCESSFUL = 0x00U,
    FUNC_ERROR      = 0x01U
} FUNC_InitTypeDef;

typedef enum {
    RESP_MSG_OK    = 0x00U,
    RESP_MSG_BUSY,
    RESP_MSG_ERROR,
    RESP_MSG_CMD,
    RESP_MSG_FAIL,
    RESP_MSG_NONE
} WIFI_RespMsgTypeDef;

typedef enum {
    NULL_MODE            = 0x00U,
    STATION_MODE         = 0x01U,
    SOFTAP_MODE          = 0x02U,
    SOFTAP_STATION_MODE  = 0x03U
} WIFI_ModeTypeDef;

typedef enum {
    SC_DISABLE = 0x00U,
    SC_ENABLE  = 0x01U
} WIFI_ScConfigTypeDef;

typedef enum { CLS_0, CLS_1 } MQTT_CC_ClsTypeDef;
typedef enum { QOS_0, QOS_1, QOS_2 } MQTT_CC_QosTypeDef;
typedef enum { RTN_0, RTN_1 } MQTT_CC_RtnTypeDef;

typedef enum {
    MQTT_TCP         = 0x01U,
    MQTT_TLS_1, MQTT_TLS_2, MQTT_TLS_3, MQTT_TLS_4,
    MQTT_WebSocket_1, MQTT_WebSocket_2, MQTT_WebSocket_3,
    MQTT_WebSocket_4, MQTT_WebSocket_5
} MQTT_UserConfigSchemeTypeDef;

typedef enum {
    POLLING_MODE   = 0x00U,
    INTERRUPT_MODE = 0x01U
} MQTT_DataRecvModeTypeDef;

typedef enum {
    TX_READY = 0x00U,
    TX_BUSY,
    RX_BUSY,
    RX_READY
} UART_RespMsgTypeDef;

/* ---- Init State Machine ----------------------------------------------- */

typedef enum {
    MQTT_INIT_STATE_WIFI_RESET      = 0x00U,
    MQTT_INIT_STATE_WIFI_MODE,
    MQTT_INIT_STATE_WIFI_SMARTCONFIG,
    MQTT_INIT_STATE_WIFI_DISC_AP,
    MQTT_INIT_STATE_WIFI_SET_AP,
    MQTT_INIT_STATE_WIFI_SET_TIME,
    MQTT_INIT_STATE_MQTT_USER_CONFIG,
    MQTT_INIT_STATE_MQTT_CONN_CONFIG,
    MQTT_INIT_STATE_MQTT_CONN,
    MQTT_INIT_STATE_END_CASE,
    MQTT_INIT_STATE_TIMEOUT
} MQTT_InitTypeDef;

typedef enum {
    MQTT_RECEIVE_PARSE_ERROR = 0x00U,
    MQTT_INIT_ERROR_WIFI_RESET,
    MQTT_INIT_ERROR_WIFI_MODE,
    MQTT_INIT_ERROR_WIFI_SMARTCONFIG,
    MQTT_INIT_ERROR_WIFI_DISC_AP,
    MQTT_INIT_ERROR_WIFI_SET_AP,
    MQTT_INIT_ERROR_WIFI_SET_TIME,
    MQTT_INIT_ERROR_MQTT_USER_CONFIG,
    MQTT_INIT_ERROR_MQTT_CONN_CONFIG,
    MQTT_INIT_ERROR_MQTT_CONN_FAIL,
    MQTT_INIT_ERROR_STATUS,
    MQTT_INIT_ERROR_FUNC_TX,
    MQTT_INIT_ERROR_FUNC_RX,
    MQTT_INIT_ERROR_FUNC_TIMEOUT,
    AWS_CERT_INIT_ERROR_SUB,
    AWS_CERT_INIT_ERROR_PUB,
    AWS_CERT_INIT_ERROR_RECEIVE,
    AWS_CERT_INIT_ERROR_PARSER,
    AWS_CERT_INIT_ERROR_REGISTER_THINGS,
    AWS_CERT_INIT_ERROR_FLASH,
    AWS_CERT_INIT_ERROR_CLEAN
} MQTT_ErrorTypeDef;

typedef enum {
    MQTT_CB_SYNC_START1 = 0x00U,
    MQTT_CB_SYNC_START2,
    MQTT_CB_SYNC_START3,
    MQTT_CB_TOPIC_CAPTURE_START,
    MQTT_CB_TOPIC_CAPTURE,
    MQTT_CB_DATASIZE_CAPTURE_START,
    MQTT_CB_DATASIZE_CAPTURE,
    MQTT_CB_DATA_CAPTURE,
    MQTT_CB_SYNC_STOP,
    MQTT_CB_PACKET_CAPTURE
} MQTT_CallbackParserTypeDef;

typedef enum {
    MQTT_SUB_INIT                = MQTT_INIT_STATE_END_CASE,
    MQTT_SUB_INIT_STATE_END_CASE = 0x10U
} MQTT_SubInitTypeDef;

typedef enum {
    MQTT_PUB_INIT                = MQTT_SUB_INIT_STATE_END_CASE,
    MQTT_PUB_INIT_STATE_END_CASE = 0x20U
} MQTT_PubInitTypeDef;

typedef enum {
    MQTT_PUB_RAW_AT_COMMAND_SEND = 0x00U,
    MQTT_PUB_RAW_AT_COMMAND_RECEIVE,
    MQTT_PUB_RAW_DATA_SEND,
    MQTT_PUB_RAW_DATA_RECEIVE,
    MQTT_PUB_RAW_END_CASE
} MQTT_PubRawDataTypeDef;

typedef enum {
    MQTT_SEND_EPC = 0x00U,
    MQTT_ERASE_EPC,
    MQTT_SEND_READER_ERROR,
    MQTT_SEND_EMPTY_SLOT,
    MQTT_SEND_FW_VERSION,
    MQTT_BLINK_LED,
    MQTT_SEND_RX_PARSE_ERROR
} MQTT_DataTypeDef;

typedef enum {
    MQTT_DIR_TX = 0x00U,
    MQTT_DIR_RX = 0x01U
} MQTT_DataDirTypeDef;

typedef enum {
    DATA_u8  = 0x01U, DATA_i8  = 0x02U,
    DATA_u16 = 0x03U, DATA_i16 = 0x04U,
    DATA_u32 = 0x05U, DATA_i32 = 0x06U,
    STRING   = 0x07U, BINARY   = 0x08U
} MQTT_FlashDataTypeDef;

/* ---- Data Structures -------------------------------------------------- */

typedef struct {
    uint16_t keep_alive;
} MQTT_ConnConfig;

typedef struct {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} MQTT_FwVersionDataTypeDef;

typedef struct {
    char    topic_id[100];
    uint8_t data_length;
    char    data[MQTT_DATA_PACKET_BUFF_SIZE + MQTT_PACKET_SIZE_WITHOUT_DATA];
} MQTT_MsgDataTypeDef;

typedef struct {
    uint8_t errorCode;
} MQTT_ErrorDataTypeDef;

typedef struct {
    char    charMacID[MAC_ID_CHAR_SIZE + 1];
    uint8_t hexMacID[MAC_ID_UINT_SIZE];
} MQTT_MacIdTypeDef;

typedef struct {
    char                       *mqttPacketBuffer;
    WIFI_ModeTypeDef            mode_wifi;
    WIFI_ScConfigTypeDef        OSC_enable;
    char                       *wifiID;
    char                       *wifiPassword;
    uint8_t                     timezone;
    MQTT_UserConfigSchemeTypeDef mode_mqtt;
    char                       *clientID;
    char                       *username;
    char                       *mqttPassword;
    uint16_t                    keepAlive;
    MQTT_CC_ClsTypeDef          cleanSession;
    MQTT_CC_QosTypeDef          qos;
    MQTT_CC_RtnTypeDef          retain;
    char                       *brokerAddress;
    uint8_t                     reconnect;
    char                       *deviceId;
    char                       *deviceId_prestring;
    char                       *deviceId_laststring;
    char                        subtopic[50];
    char                        pubtopic[50];
} MQTT_Config;

typedef struct {
    int   status;
    int   application;
    char *command;
} GenericState;

typedef struct {
    long timestamp;
} TimeData;

typedef struct {
    TimeData status;
    TimeData application;
    TimeData command;
} MetadataState;

typedef struct {
    GenericState desired;
    GenericState reported;
    GenericState delta;
} State;

typedef struct {
    State         state;
    MetadataState metadata;
    int           version;
    long          timestamp;
} MQTTShadow;

/* ---- Shared variables (defined in mqtt_core.c) ------------------------ */

extern char              mqttPacketBuffer[MQTT_DATA_PACKET_BUFF_SIZE];
extern char              mqttDataBuffer[1];
extern volatile uint8_t  flag_mqtt_error;
extern volatile uint8_t  flag_mqtt_init_done;
extern volatile uint8_t  flag_mqtt_rx_done;
extern volatile uint8_t  flag_mqtt_connect;
extern volatile uint8_t  mqtt_timer;
extern volatile uint8_t  mqtt_timer_en;
extern volatile uint8_t  capturedPacketDataSize;
extern MQTT_ErrorDataTypeDef mqttErrorData;

/* ---- Public API (Platform Independent) -------------------------------- */

/* WiFi + MQTT Initialization */
FUNC_InitTypeDef MQTT_Init(MQTT_Config *config);

/* Low-level WiFi AT command wrappers */
FUNC_StatusTypeDef Wifi_Reset2(char *buffer, MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_SetWifiMode2(char *buffer, WIFI_ModeTypeDef mode, MQTT_DataRecvModeTypeDef recvMode);
FUNC_StatusTypeDef Wifi_StartSmartConfig2(char *buffer, MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_StopSmartConfig2(char *buffer, MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_QAP2(char *buffer, MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_SetAP2(char *buffer, char *ssid, char *password, MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_SetTime2(char *buffer, uint8_t timezone, MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_MqttUserConfig2(char *buffer, MQTT_UserConfigSchemeTypeDef mode,
                                        char *clientID, char *username, char *password,
                                        MQTT_DataRecvModeTypeDef recvMode);
FUNC_StatusTypeDef Wifi_MqttConnConfig2(char *buffer, uint16_t keepAlive, MQTT_CC_ClsTypeDef cleanSession,
                                        MQTT_CC_QosTypeDef qos, MQTT_CC_RtnTypeDef retain,
                                        MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_MqttConn2(char *buffer, char *brokerAddress, uint8_t reconnect,
                                   MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_GetMqttConn2(char *buffer, MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_GetMAC(char *buffer);

/* MQTT Subscribe / Publish */
FUNC_StatusTypeDef Wifi_MqttSub2(char *buffer, const char *topic, uint8_t qos, MQTT_DataRecvModeTypeDef mode);
FUNC_StatusTypeDef Wifi_MqttSubInit(char *buffer, const char *topic, uint8_t QoS);
FUNC_StatusTypeDef Wifi_MqttPub2(char *buffer, const char *topic, const char *data, uint8_t respSize,
                                  MQTT_CC_QosTypeDef qos, MQTT_CC_RtnTypeDef retain,
                                  MQTT_DataRecvModeTypeDef mode);
void Wifi_MqttPubInit(char *buffer, const char *topic, MQTT_MacIdTypeDef *deviceID,
                      MQTT_FwVersionDataTypeDef *fwVersion, MQTT_CC_QosTypeDef qos, MQTT_CC_RtnTypeDef retain);
FUNC_StatusTypeDef Wifi_MqttPubRaw2(char *buffer, char *topic, uint16_t dataSize, char *data,
                                     MQTT_CC_QosTypeDef qos, MQTT_CC_RtnTypeDef retain,
                                     MQTT_DataRecvModeTypeDef mode);

/* UART command/receive helpers */
MqttPort_Status    Wifi_SendCommand(const char *cmd);
MqttPort_Status    Wifi_SendCommand2(const char *cmd);
MqttPort_Status    Wifi_Receive(char *buffer, uint16_t len, uint16_t timeout, MQTT_DataRecvModeTypeDef mode);
WIFI_RespMsgTypeDef Wifi_CheckResponse(char *buffer, char *response);
UART_RespMsgTypeDef UART_CheckResponse(void);

/* Callback / Parser */
void UART_MqttCallbackPacketCapture(const char *dataBuffer, char *packetBuffer);
void UART_MqttPacketParser(MQTT_MsgDataTypeDef *messageData, const char *dataPacket, uint16_t dataSize);
void UART_MqttSubRecvParser(MQTT_MsgDataTypeDef *messageData, const char *dataBuffer, uint16_t dataBufferSize);

/* Utilities */
void parse_wifi_info(char *buffer, char *ssid, char *password);
void charToInt(uint8_t *charArray, uint8_t *intNum, uint8_t length);
void getUniqueID3(char charUniqueID[], const char *buffer);
void Wifi_WaitMqttData(void);

/* LED helpers (use port GPIO abstraction) */
void LED_MqttTXBlink(void);
void LED_MqttRXBlink(void);
void LED_Mqttconnected(uint8_t set_led);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_CORE_H */
