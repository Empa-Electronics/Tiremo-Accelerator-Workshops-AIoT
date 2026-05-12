#include "EMPA_MqttAws.h"
#include <string.h>

MQTT_Config mqttConfig = {
    .mqttPacketBuffer = mqttPacketBuffer,
    .mode_wifi = STATION_MODE,
    .OSC_enable = SC_DISABLE,
    .wifiID = "EMPA_ARGE_4G",
    .wifiPassword = "Empa1982",
    .timezone = 3,
    .mode_mqtt = MQTT_TLS_1,
    .clientID = "GW-001",
    .username = "Empa123",
    .mqttPassword = "159753456Empa",
    .keepAlive = 300,
    .cleanSession = CLS_1,
    .qos = QOS_0,
    .retain = RTN_0,
    .brokerAddress = "b66f521753f14834b95b7bc626ce47a5.s1.eu.hivemq.cloud",
    .reconnect = 0,
    .subtopic = "devices/GW-001/cmd/config",
    .pubtopic = "devices/GW-001/tele/data_batch"
};

uint8_t MQTT_ConnectBroker(void)
{
    uint8_t tryCount = 0;

    while (tryCount < MAX_TRY_FUNC)
    {
        if (MQTT_Init(&mqttConfig) == FUNC_SUCCESSFUL)
        {
            LED_Mqttconnected(1);

            /* Publish connection confirmation */
            char msg[] = "mqtt successful";
            memset(mqttPacketBuffer, 0, MQTT_DATA_PACKET_BUFF_SIZE);
            Wifi_MqttPubRaw2(mqttPacketBuffer, mqttConfig.pubtopic,
                (uint16_t)strlen(msg), msg, QOS_0, RTN_0, POLLING_MODE);
            LED_MqttTXBlink();
            return 0;
        }
        tryCount++;
    }

    LED_Mqttconnected(0);
    return 1;
}

void MQTT_PublishSensorData(const SensorData_t *pData)
{
    static char jsonBuf[256];

    uint16_t len = Sensor_FormatJSON(pData, jsonBuf, sizeof(jsonBuf));

    memset(mqttPacketBuffer, 0, MQTT_DATA_PACKET_BUFF_SIZE);
    LED_MqttTXBlink();
    Wifi_MqttPubRaw2(mqttPacketBuffer, mqttConfig.pubtopic,
        len, jsonBuf, QOS_0, RTN_0, POLLING_MODE);
}

void MY_MqttAwsProcess(void)
{
    if (MQTT_ConnectBroker() == 0)
    {
        char msg[] = "mqtt successful";
        memset(mqttPacketBuffer, 0, MQTT_DATA_PACKET_BUFF_SIZE);
        LED_MqttTXBlink();
        Wifi_MqttPubRaw2(mqttPacketBuffer, mqttConfig.pubtopic,
            (uint16_t)strlen(msg), msg, QOS_0, RTN_0, POLLING_MODE);
    }
}
