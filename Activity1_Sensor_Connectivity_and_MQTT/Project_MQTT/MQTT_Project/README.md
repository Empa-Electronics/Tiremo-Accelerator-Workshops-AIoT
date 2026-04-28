<p align="center">
    <img src="../../../Additionals/Empa-Accelerator-Workshops-Template-Banner.png" alt="Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

## İçindekiler
- [1. Sensor Tutorial Demo](#1-sensor-tutorial-demo)
- [2. MQTT AWS DEMO](#2-mqtt-aws-demo)
   
# 1. Sensor Tutorial Demo

Tanım: Bu demo, Tiremo®Cortex üzerindeki sensörlerin başlangıç (initialization) ve veri işleme (process) işlemleri ile sensörlerden veri toplar ve bu verileri seri terminal üzerinden görüntüler.Tanım: Bu demo, Tiremo®Cortex üzerindeki sensörlerin başlangıç (initialization) ve veri işleme (process) işlemleri ile sensörlerden veri toplar ve bu verileri seri terminal üzerinden görüntüler.

Bu demoyu derleyip çalıştırabilmek için main.c dosyası içerisindeki 
```c
/* EMPA Sensor Tutorial Demo Start */
/* EMPA Sensor Tutorial Demo End */
```
yorum satırları arasındaki 
```c
//#define EMPA_SENSOR_INIT
//#define EMPA_SENSOR_PROCESS
//#define EMPA_SENSOR_PRINT
```
tanımlarının yorum satırından çıkarılmış halde olması gerekmektedir. Bu işlem, satırların başındaki "//" ifadeleri kaldırarak yapılabilir.

Ayrıca
```c
/* EMPA MQTT Demo Start */
/* EMPA MQTT Demo End */
```
yorum satırları arasındaki 
```c
//#define EMPA_SENSOR_INIT
//#define EMPA_MQTT_AWS
```
tanımlarının yorum satırına alınması gerekmektedir. Bu işlem, satırların başına "//" ifadeleri eklenerek yapılabilir.

# 2. MQTT AWS DEMO

Tanım: Bu demo, Tiremo®Cortex üzerindeki sıcaklık ve nem sensöründen verileri alır ve bu verileri, AWS tabanlı HiveMQ MQTT Broker'ına 5 saniyede bir gönderir.


Bu demoyu derleyip çalıştırabilmeniz için main.c dosyası içerisindeki 
```c
/* EMPA MQTT Demo Start */
/* EMPA MQTT Demo End */
```
yorum satırları arasındaki 
```c
//#define EMPA_SENSOR_INIT
//#define EMPA_MQTT_AWS
```
tanımlarının yorum satırından çıkarılmış halde olması gerekmektedir. Bu işlem, satırların başındaki "//" ifadeleri kaldırarak yapılabilir.

Ayrıca
```c
/* EMPA Sensor Tutorial Demo Start */
/* EMPA Sensor Tutorial Demo End */
```
yorum satırları arasındaki 
```c
#define EMPA_SENSOR_INIT
#define EMPA_SENSOR_PROCESS
#define EMPA_SENSOR_PRINT
```
tanımlarının yorum satırına alınmış olması gerekmektedir. Bu işlem, satırların başına "//" ifadeleri eklenerek yapılabilir.

