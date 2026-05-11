#include "EMPA_MqttAws.h"
#include <string.h>

MQTT_Config mqttConfig = {
    .mqttPacketBuffer = mqttPacketBuffer,
    .mode_wifi = STATION_MODE,
    .OSC_enable = SC_DISABLE,
    .wifiID = "EMPA_Arge",
    .wifiPassword = "Emp@Arg2024!",
    .timezone = 3,
    .mode_mqtt = MQTT_TLS_1,
    .clientID = "GW-001",
    .username = "sinan1234",
    .mqttPassword = "Sinan1234",
    .keepAlive = 300,
    .cleanSession = CLS_1,
    .qos = QOS_0,
    .retain = RTN_0,
    .brokerAddress = "bdc8928bfa1b4adf8c41e7f435557167.s1.eu.hivemq.cloud",
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
            LED_MqttTXBlink();
            Wifi_MqttPubRaw2(mqttPacketBuffer, mqttConfig.pubtopic,
                (uint16_t)strlen(msg), msg, QOS_0, RTN_0, POLLING_MODE);
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
