<p align="center">
    <img src="../../../Additionals/Empa-Accelerator-Workshops-Template-Banner.png" alt="Tiremo® Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

## Project_DataCollector — Tiremo®Cortex Veri Toplama Projesi

Bu proje, Tiremo®Cortex üzerindeki ISM330IS IMU sensöründen ham ivmeölçer ve jiroskop verisi toplayarak seri port üzerinden bilgisayara aktarır. Toplanan veriler, makine öğrenimi modelinin eğitim ve doğrulama aşamalarında kullanılmak üzere veri seti oluşturmaya yarar.

---

## Proje Yapısı

```
Project_DataCollector/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── custom_bus.h
│   │   └── ...
│   └── Src/
│       ├── main.c          ← Ana uygulama mantığı
│       ├── custom_bus.c
│       └── ...
├── Drivers/
│   ├── BSP/
│   ├── CMSIS/
│   └── STM32U5xx_HAL_Driver/
├── X-CUBE-ISPU/
└── WORKSHOP_DataCollector.ioc
```

---

## Ne Yapar?

- ISM330IS IMU sensörünü başlatır ve yapılandırır (ivmeölçer / jiroskop).
- Seçilen sensör moduna göre (`ONLY_ACCELEROMETER`, `ONLY_GYRO`, `ACCELEROMETER_AND_GYRO`) 128 örneklik tamponlar doldurulur.
- Her tampon dolduğunda, ham veriler boşlukla ayrılmış biçimde UART1 üzerinden seri porta yazdırılır.
- SHT40 sıcaklık/nem sensörü ve IMP34DT05 mikrofon çevre birimi de projede desteklenmektedir.

### Sensör Modu

`main.c` içinde aşağıdaki satırı değiştirerek aktif sensör modunu seçebilirsiniz:

```c
SensorMode sensor_mode = ONLY_ACCELEROMETER;
```

| Mod | Açıklama |
|-----|----------|
| `ONLY_ACCELEROMETER` | Yalnızca X, Y, Z ivme eksenleri — 384 değer |
| `ONLY_GYRO` | Yalnızca X, Y, Z jiroskop eksenleri — 384 değer |
| `ACCELEROMETER_AND_GYRO` | Her iki sensör birlikte — 768 değer |

---

## Derleme ve Yükleme

1. STM32CubeIDE'yi açın.
2. `File → Open Projects from File System` menüsünden bu klasörü içe aktarın.
3. Projeyi derleyip Tiremo®Cortex üzerine yükleyin.
4. UART1 çıkışını izlemek için bir seri terminal açın (baud: **115200**).

---

## Veri Formatı

Seri port çıktısı her satırda bir tampon içerecek şekilde boşlukla ayrılmış tam sayılardan oluşur:

```
-312 45 9810 -298 51 9823 ...
```

Bu çıktı doğrudan `Activity2` Jupyter Not Defteri'ndeki veri yükleme adımıyla uyumludur.

---

## Sonraki Adım

Veri toplandıktan sonra makine öğrenimi modelini eğitmek için Activity-2 Not Defteri'ni kullanın. Eğitim tamamlandığında modeli **Project_EdgeAI** projesine entegre ederek Tiremo®Cortex üzerinde gerçek zamanlı çıkarım yapabilirsiniz.

### ↳ [Edge-AI Entegrasyon Kılavuzu](../../EdgeAI_Entegrasyon.md)
DataCollector projesini Edge-AI projesine dönüştürme adımlarını içerir.
