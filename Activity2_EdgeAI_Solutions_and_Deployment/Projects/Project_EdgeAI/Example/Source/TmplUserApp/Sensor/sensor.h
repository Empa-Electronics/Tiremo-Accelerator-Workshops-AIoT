#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>

/* Sensor data structure - all readings in one place */
typedef struct {
    /* SHT40 */
    int32_t  temperature_mC;    /* milli-Celsius (e.g. 25000 = 25.000 C) */
    int32_t  humidity_mRH;      /* milli-%RH     (e.g. 50000 = 50.000 %) */
    uint8_t  sht40_ok;

    /* Battery */
    uint32_t battery_mV;        /* millivolts (e.g. 3300 = 3.300 V) */
    uint8_t  battery_ok;

    /* LIS2DE12 Accelerometer */
    int16_t  accel_x_mg;        /* milli-g */
    int16_t  accel_y_mg;
    int16_t  accel_z_mg;
    uint8_t  lis2de12_ok;

    /* MP23ABS1 Microphone */
    uint32_t mic_rms;           /* RMS value of audio buffer */
    uint8_t  mic_ok;
} SensorData_t;

/**
 * @brief  Initialize all sensors and report pass/fail
 * @return Number of failed sensors (0 = all OK)
 */
uint8_t Sensor_TestAll(void);

/**
 * @brief  Read all sensors and print results to debug terminal
 * @return Pointer to internal SensorData_t (valid until next call)
 */
SensorData_t* Sensor_ReadAndPrint(void);

/**
 * @brief  Run LED startup test sequence (lights each LED in turn)
 * @return None
 */
void Sensor_LEDTest(void);

/**
 * @brief  Format sensor data as JSON string
 * @param  pData   Sensor data to format
 * @param  outBuf  Output buffer for JSON string
 * @param  bufLen  Output buffer size
 * @return Length of JSON string written
 */
uint16_t Sensor_FormatJSON(const SensorData_t *pData, char *outBuf, uint16_t bufLen);

#endif /* SENSOR_H_ */
