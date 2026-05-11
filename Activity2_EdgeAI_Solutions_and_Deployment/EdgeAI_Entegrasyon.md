<p align="center">
    <img src="../Additionals/Empa-Accelerator-Workshops-Template-Banner.png" alt="Tiremo® Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

## Edge-AI Entegrasyon Kılavuzu: DataCollector → EdgeAI

Bu kılavuz, **Project_DataCollector** projesine Tiremo® tarafından üretilmiş bir AI modelini entegre ederek **Project_EdgeAI**'ya dönüştürme adımlarını açıklar. Üç temel değişiklik gereklidir: `tiremo_api` klasörünün eklenmesi, `CMakeLists.txt` dosyasının güncellenmesi ve `prv_user_code.c` dosyasının düzenlenmesi.

---

## Ön Koşullar

- Activity-2 Not Defteri'nde model eğitimi ve dönüşümü tamamlanmış olmalıdır.
- Tiremo®Intelligence tarafından üretilen `tiremo_api/` klasörü mevcut olmalıdır.  
  Bu klasör aşağıdaki dört dosyayı içerir:

| Dosya | Açıklama |
|-------|----------|
| `tiremo_api.c` | `tiremo_ai_classify_label()` fonksiyonunun gerçekleştirimi |
| `tiremo_api.h` | API tanımları ve `TiremoStatus` dönüş kodları |
| `tiremo_classes.h` | Sınıf sayısı (`TIREMO_N_CLASSES`), enum ve etiket dizisi |
| `tiremo_config.h` | Backend seçimi (`TIREMO_BACKEND`) ve özellik sayısı (`TIREMO_N_FEATURES`) |

---

## Adım 1 — `tiremo_api/` Klasörünü Projeye Ekleyin

1. Aktivite 2 klasörü içerisinden, Tiremo®Intelligence tarafından üretilen `tiremo_api/` klasörünü  kopyalayın.
2. Klasörü `Project_DataCollector/` kök dizinine yapıştırın.

Sonuç dizin yapısı:

```
Project_DataCollector/
├── Example/
├── Framework/
├── Platform/
├── tiremo_api/          ← YENİ
│   ├── tiremo_api.c
│   ├── tiremo_api.h
│   ├── tiremo_classes.h
│   ├── tiremo_config.h
│   └── model.h          ← Eğitilmiş model dosyası (Google Colab çıktısı)
├── CMakeLists.txt
└── flash.bat
```

---

## Adım 2 — `prv_user_code.c` — Include Ekleyin

`Example/Source/TmplUserApp/prv_user_code.c` dosyasındaki `/* ---------- AI INCLUDE STARTS HERE --------- */` bloğuna aşağıdaki satırı ekleyin:

```c
/* ---------- AI INCLUDE STARTS HERE --------- */
#include "tiremo_api.h"
/* ---------- AI INCLUDE PATH ENDS HERE --------- */
```

---

## Adım 3 — `prv_user_code.c` — Global Değişken Ekleyin

Aynı dosyada `/* ---------- AI GLOBALS STARTS HERE --------- */` bloğuna aşağıdaki değişkeni ekleyin:

```c
/* ---------- AI GLOBALS STARTS HERE --------- */
static const char *label;
/* ---------- AI GLOBALS ENDS HERE --------- */
```

| Değişken | Tür | Açıklama |
|----------|-----|----------|
| `label` | `const char *` | En büyük olasılığa sahip sınıfın etiketi |

---

## Adım 4 — `prv_user_code.c` — Çıkarım Bloğunu Ekleyin

`PRV_USER_Code()` fonksiyonu içindeki `/* ---------- AI INFERENCE STARTS HERE --------- */` bloğuna çıkarım kodunu ekleyin:

**Önce (DataCollector):**

```c
        for (int i = 0; i < 128; i++)
        {
            DebugFramework_Printf("%d %d %d", data[i * 3 + 0], data[i * 3 + 1], data[i * 3 + 2]);
        }
        DebugFramework_Printf("\n\r");

        /* ---------- AI INFERENCE STARTS HERE --------- */
        
        /* ---------- AI INFERENCE ENDS HERE --------- */
```

**Sonra (EdgeAI):**

```c
        for (int i = 0; i < 128; i++)
        {
            DebugFramework_Printf("%d %d %d", data[i * 3 + 0], data[i * 3 + 1], data[i * 3 + 2]);
        }
        DebugFramework_Printf("\n\r");

        /* ---------- AI INFERENCE STARTS HERE --------- */
        int result = tiremo_ai_classify_label(data, 384, &label);
        if (result == 0 && label != NULL)
        {
            DebugFramework_Printf("Classification: %s\r\n", label);
        }
        /* ---------- AI INFERENCE ENDS HERE --------- */
```

> **Not:** `data` dizisi `int16_t data[384]` olarak tanımlıdır ve 128 örneklik bir pencerede 3 eksenden (X, Y, Z) toplanan ham ivmeölçer verilerini içerir (128 × 3 = 384 değer). Model bu 384 özelliği girdi olarak bekler; dönüşüm veya ölçekleme gerekmez.

---

## Adım 5 — `CMakeLists.txt` Güncelleme

VS Code + CMake derleme sisteminin `tiremo_api/` dosyalarını bulabilmesi için iki yer güncellenmelidir.

### 5a. Kaynak Dosyaları Ekleyin

`CMakeLists.txt` dosyasında `# --------- AI SOURCES STARTS HERE ---------` bloğunu bulun ve aşağıdaki satırları ekleyin:

```cmake
# --------- AI SOURCES STARTS HERE ---------
set(TIREMO_API_DIR "${CMAKE_SOURCE_DIR}/tiremo_api")
file(GLOB TIREMO_API_FILES CONFIGURE_DEPENDS
    "${TIREMO_API_DIR}/*.c"
    "${TIREMO_API_DIR}/*.h"
)
list(APPEND USER_LIB_SOURCES ${TIREMO_API_FILES})
# --------- AI SOURCES ENDS HERE ---------
```

### 5b. Include Yolu Ekleyin

Aynı dosyada `# ---------- AI INCLUDE PATH STARTS HERE ---------` bloğunu bulun ve aşağıdaki satırı ekleyin:

```cmake
    # ---------- AI INCLUDE PATH STARTS HERE ---------
    ${CMAKE_SOURCE_DIR}/tiremo_api
    # ---------- AI INCLUDE PATH ENDS HERE ---------
```

---



## Değişiklik Özeti

```
tiremo_api/ (yeni klasör)                            ← Adım 1
  [+] tiremo_api.c  
  [+] tiremo_api.h  
  [+] tiremo_classes.h
  [+] tiremo_config.h
  [+] model.h

prv_user_code.c
  [+] #include "tiremo_api.h"                        ← Adım 2
  [+] static const char *label                       ← Adım 3
  [+] tiremo_ai_classify_label() çıkarım bloğu       ← Adım 4

CMakeLists.txt
  [+] TIREMO_API_DIR kaynak glob'u                   ← Adım 5a
  [+] ${CMAKE_SOURCE_DIR}/tiremo_api include yolu    ← Adım 5b
```



---

## Doğrulama

1. Tüm değişiklikler tamamlandıktan sonra projeyi hatasız derleyin ve yükleyin:
   ```powershell
   .\flash.bat
   ```
2. Seri terminali 115200 baud ile açın.
3. Tiremo®Cortex'i elinize alıp bir hareket gerçekleştirin:
   - Seri porta `CIRCLE`, `HORIZONTAL`, `STANDBY`, `TRIANGLE` veya `VERTICAL` etiketlerinden biri basılmalıdır.

---

## Kaynaklar

- [emlearn: Machine Learning for Embedded Systems](https://emlearn.readthedocs.io/)
