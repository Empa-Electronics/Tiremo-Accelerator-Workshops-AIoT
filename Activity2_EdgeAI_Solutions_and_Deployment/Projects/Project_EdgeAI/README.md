<p align="center">
    <img src="../../../Additionals/Empa-Accelerator-Workshops-Template-Banner.png" alt="Tiremo® Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

## Project_EdgeAI — Tiremo®Cortex Gerçek Zamanlı Edge-AI Projesi

Bu proje, **Project_DataCollector** üzerine inşa edilmiştir. Toplanan IMU verilerini bir Random Forest modeline besleyerek Tiremo®Cortex üzerinde **gerçek zamanlı el hareketi tanıma** çıkarımı yapar. Model çıktısı UART1 üzerinden seri porta yazdırılır.

---

## Proje Yapısı

```
Project_EdgeAI/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── custom_bus.h
│   │   └── ...
│   └── Src/
│       ├── main.c          ← AI çıkarım mantığı eklenmiş ana uygulama
│       ├── custom_bus.c
│       └── ...
├── tiremo_api/             ← Tiremo® AI çalışma zamanı (DataCollector'da yoktur)
│   ├── tiremo_api.c
│   ├── tiremo_api.h
│   ├── tiremo_classes.h    ← Sınıf isimleri ve sayısı
│   └── tiremo_config.h     ← Backend ve özellik boyutu yapılandırması
├── Drivers/
│   ├── BSP/
│   ├── CMSIS/
│   └── STM32U5xx_HAL_Driver/
├── X-CUBE-ISPU/
└── WORKSHOP_DataCollector.ioc
```

---

## DataCollector Projesinden Farkları

| # | Fark | Açıklama |
|---|------|----------|
| 1 | `tiremo_api/` klasörü | AI çalışma zamanı, model tanımları ve sınıf etiketlerini içerir |
| 2 | `main.c` — include | `#include <tiremo_api.h>` satırı eklenmiştir |
| 3 | `main.c` — değişkenler | `tiremo_input[]`, `tiremo_probabilities[]`, `tiremo_label_out` tanımlanmıştır |
| 4 | `main.c` — çıkarım döngüsü | `ACCELEROMETER_AND_GYRO` modunda `tiremo_ai_classify_label()` çağrısı yapılmaktadır |
| 5 | CubeIDE yol tanımları | `tiremo_api/` klasörü kaynak ve include yolu olarak projeye eklenmiştir |

---

## Tanınan El Hareketleri

Model aşağıdaki 5 el hareketini sınıflandırır:

| Sınıf | Etiket |
|-------|--------|
| 0 | `CIRCLE` |
| 1 | `HORIZONTAL` |
| 2 | `STANDBY` |
| 3 | `TRIANGLE` |
| 4 | `VERTICAL` |

<img src="../../Additionals/Hand-Characters.png" alt="El Hareketi Sınıfları" width="800"/>

---

## Çıkarım Akışı

```
IMU (ISM330IS)
    │
    ▼
fill_accelerator_and_gyro_buffer()   ← 768 örneklik buffer doldurulur
    │
    ▼
tiremo_input[] dizisine kopyala      ← int32_t → double dönüşümü
    │
    ▼
tiremo_ai_classify_label()           ← Random Forest ile sınıf tahmini
    │
    ▼
printf("%s\r\n", tiremo_label_out)   ← Seri port çıktısı
```

---

## Çıkarımla İlgili Yapılandırma (`tiremo_config.h`)

```c
#define TIREMO_N_FEATURES   768     /* 128 örnek × 6 eksen   */
```

---

## Derleme ve Yükleme

1. STM32CubeIDE'yi açın.
2. `File → Open Projects from File System` menüsünden bu klasörü içe aktarın.
3. Projeyi derleyip Tiremo®Cortex'e flash'layın.
4. UART çıkışını 115200 baud ile izleyin.
5. Tiremo®Cortex'i elinize alın ve hareketi gerçekleştirin:
    - Seri porta `CIRCLE`, `HORIZONTAL`, `STANDBY`, `TRIANGLE` veya `VERTICAL` etiketlerinden biri basılmalıdır.

---

## DataCollector Projesini EdgeAI Projesine Dönüştürme

Sıfırdan başlamak yerine mevcut DataCollector projesine AI çıkarımı eklemek için aşağıdaki kılavuzu izleyebilirsiniz:

### ↳ [Edge-AI Entegrasyon Kılavuzu](../../EdgeAI_Entegrasyon.md)
