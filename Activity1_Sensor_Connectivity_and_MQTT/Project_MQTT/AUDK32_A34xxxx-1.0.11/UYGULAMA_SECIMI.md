# Uygulama Seçimi — EMPA Feature Flags

Dosya: `Example/Source/TmplUserApp/prv_user_code.c`

---

## Nasıl Açılır / Kapatılır?

Dosyanın en üstünde (include'lardan hemen sonra) iki satır bulunur:

```c
/* Feature Selection - comment out to disable */
#define EMPA_SENSOR_PROCESS   /* Print sensor data to terminal */
#define EMPA_MQTT_AWS         /* Publish sensor data to MQTT broker */
```

Bir modu **kapatmak** için satırın başına `//` koy.

---

## Uygulama 1 — Sadece Sensör / Terminal Log

Sensör verilerini her 5 saniyede bir terminal ekranına basar.  
MQTT bağlantısı **kurulmaz**, ESP32 AT testi **atlanır**.

```c
#define EMPA_SENSOR_PROCESS   /* aktif: ekrana log */
// #define EMPA_MQTT_AWS      /* kapalı */
```

**Davranış:**
- Sıcaklık, nem, batarya, ivme, mikrofon RMS terminale basılır
- Döngü süresi: 5 saniye

---

## Uygulama 2 — Sadece MQTT / AWS

Sensör verilerini her 1 saniyede bir MQTT broker'a gönderir.  
Terminal ekranına hiçbir sensör verisi **basılmaz**.

```c
// #define EMPA_SENSOR_PROCESS   /* kapalı */
#define EMPA_MQTT_AWS            /* aktif: MQTT'ye gönder */
```

**Davranış:**
- LED6 (PB12) yanar → WiFi bağlandı
- LED7 (PB13) yanar → MQTT bağlandı
- LED5 (PB11) 500 ms yanıp söner → Her MQTT publish
- Döngü süresi: 1 saniye

---

## Her İkisi Birden (Varsayılan)

Hem terminale log basar hem de MQTT'ye gönderir.

```c
#define EMPA_SENSOR_PROCESS   /* aktif */
#define EMPA_MQTT_AWS         /* aktif */
```

**Davranış:**
- Hem terminal log hem MQTT publish
- Döngü süresi: 1 saniye

---
