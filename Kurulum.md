<p align="center">
    <img src="./Additionals/Empa-Accelerator-Workshops-Template-Banner.png" alt="Tiremo® Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

# ABOV A34G43x — Geliştirme Ortamı Kurulum Kılavuzu

Bu kılavuzu okuyan kişi, sıfırdan geliştirme ortamını kurarak ABOV A34G43x mikrodenetleyicisine kod derleyip yükleyebilir hale gelecektir.

---

## İçindekiler

1. [Gereksinimler](#1-gereksinimler)
2. [Visual Studio Code Kurulumu](#2-visual-studio-code-kurulumu)
3. [CMake Kurulumu](#3-cmake-kurulumu)
4. [Ninja Kurulumu](#4-ninja-kurulumu)
5. [ARM GCC Toolchain Kurulumu](#5-arm-gcc-toolchain-kurulumu)
6. [PATH Doğrulaması](#6-path-doğrulaması)
7. [VS Code Eklentileri](#7-vs-code-eklentileri)
8. [Projeyi Açma](#8-projeyi-açma)
9. [Derleme (Build)](#9-derleme-build)
10. [Mikrodenetleyiciye Yükleme (Flash)](#10-mikrodenetleyiciye-yükleme-flash)
11. [Tek Komutla Derleme ve Yükleme](#11-tek-komutla-derleme-ve-yükleme)
12. [Proje Yapısı](#12-proje-yapısı)
13. [Sık Karşılaşılan Hatalar](#13-sık-karşılaşılan-hatalar)
14. [Debug Mesajlarını Görme (Seri Terminal)](#14-debug-mesajlarını-görme-seri-terminal)
15. [Google Colab ile Çalışma](#15-google-colab-ile-çalışma)

---

## 1. Gereksinimler

Aşağıdaki araçların kurulu olması gerekmektedir:

| Araç | Versiyon | Açıklama |
|---|---|---|
| Visual Studio Code | En güncel | Kod editörü |
| CMake | 3.20 veya üzeri | Build sistemi |
| Ninja | En güncel | Hızlı derleme motoru |
| ARM GCC Toolchain | 13.x | arm-none-eabi-gcc cross compiler |
| Debug Kartı (CMSIS-DAP) | — | ABOV debug board, DAPLink vb. |

> **Not:** OpenOCD bu projeye dahildir (`Tool/OpenOCD/`), ayrıca kurulum gerekmez.

---

## 2. Visual Studio Code Kurulumu

1. Tarayıcıda şu adrese gidin: https://code.visualstudio.com
2. **Download for Windows** butonuna tıklayın.
3. İndirilen `.exe` dosyasını çalıştırın.
4. Kurulum sırasında **"Add to PATH"** seçeneğinin işaretli olduğundan emin olun.
5. Kurulum tamamlandıktan sonra VS Code'u açın.

---

## 3. CMake Kurulumu

### Yöntem A — winget ile (Önerilen)

PowerShell veya Komut İstemi'ni **Yönetici olarak** açın ve çalıştırın:

```powershell
winget install Kitware.CMake
```

> **Önemli:** `winget` CMake'i sessiz modda (WiX MSI) kurar ve **PATH'e otomatik eklemeyebilir**. Kurulumdan sonra Bölüm 6'daki script ile PATH'e ekleyin.

### Yöntem B — Manuel kurulum

1. https://cmake.org/download adresine gidin.
2. **Windows x64 Installer** (`.msi`) dosyasını indirin.
3. Kurulum sırasında **"Add CMake to the system PATH for all users"** seçeneğini seçin.

### Doğrulama

```powershell
cmake --version
```

Çıktı şuna benzer olmalı:
```
cmake version 3.xx.x
```

---

## 4. Ninja Kurulumu

### Yöntem A — winget ile (Önerilen)

```powershell
winget install Ninja-build.Ninja
```

> **Önemli:** `winget` ile kurulan Ninja `portable (zip)` tipinde kurulur — `AppData\Local\Microsoft\WinGet\Packages\...` altına çıkarılır ve **PATH'e otomatik eklemeyebilir**. Kurulumdan sonra Bölüm 6'daki script ile PATH'e ekleyin.

### Yöntem B — Manuel kurulum

1. https://github.com/ninja-build/ninja/releases adresine gidin.
2. En son sürümün altındaki `ninja-win.zip` dosyasını indirin.
3. Zip'i açın, `ninja.exe` dosyasını çıkarın.
4. `ninja.exe` dosyasını `C:\tools\ninja\` gibi bir klasöre koyun.
5. Bu klasörü sistem PATH'ine ekleyin:
   - `Windows Arama` → **"Sistem ortam değişkenleri"** → **Ortam Değişkenleri**
   - `Path` satırını seçip **Düzenle** → **Yeni** → klasör yolunu ekleyin.

### Doğrulama

```powershell
ninja --version
```

---

## 5. ARM GCC Toolchain Kurulumu

Bu, C kodunu mikrodenetleyici için derleyen cross-compiler'dır.

### Yöntem A — winget ile (Önerilen)

```powershell
winget install Arm.GnuArmEmbeddedToolchain
```

> **Önemli:** `winget` toolchain'i sessiz modda (NSIS) kurar ve **PATH'e otomatik eklemeyebilir**. Kurulumdan sonra Bölüm 6'daki script ile PATH'e ekleyin.

### Yöntem B — Manuel kurulum

1. https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads adresine gidin.
2. **Windows (mingw-w64-i686)** sütunundan **arm-none-eabi** satırındaki `.exe` installer'ı indirin.
3. Kurulumu başlatın.
4. Son ekranda **"Add path to environment variable"** seçeneğini işaretleyin.

### Doğrulama

```powershell
arm-none-eabi-gcc --version
```

Çıktı şuna benzer olmalı:
```
arm-none-eabi-gcc (Arm GNU Toolchain 13.x) 13.x.x
```

---

## 6. PATH Doğrulaması

Tüm araçları kurduktan sonra **yeni bir** PowerShell penceresi açın ve sırayla çalıştırın:

```powershell
cmake --version
ninja --version
arm-none-eabi-gcc --version
```

Üçü de versiyon bilgisi döndürüyorsa kurulum tamamdır.

> **Hata alırsanız:** PowerShell'i kapatıp yeniden açın. Hâlâ hata alırsanız aşağıdaki PowerShell komutunu çalıştırın. Komut, araçları kurulu oldukları yerde otomatik arayıp bulur ve kullanıcı PATH'ine ekler:

```powershell
@(
    (Get-ChildItem "$env:ProgramFiles", "${env:ProgramFiles(x86)}" -Filter "cmake.exe"      -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1).DirectoryName,
    (Get-ChildItem "$env:ProgramFiles", "${env:ProgramFiles(x86)}",
                  "$env:LOCALAPPDATA\Microsoft\WinGet\Packages",
                  "$env:LOCALAPPDATA\Programs" -Filter "ninja.exe"              -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1).DirectoryName,
    (Get-ChildItem "$env:ProgramFiles", "${env:ProgramFiles(x86)}" -Filter "arm-none-eabi-gcc.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1).DirectoryName
) | Where-Object { $_ } | ForEach-Object {
    $p = [System.Environment]::GetEnvironmentVariable("PATH", "User")
    if ($p -notlike "*$_*") {
        [System.Environment]::SetEnvironmentVariable("PATH", "$p;$_", "User")
        Write-Host "Eklendi: $_"
    } else {
        Write-Host "Zaten mevcut: $_"
    }
}
Write-Host "Tamamlandi. Yeni bir PowerShell penceresi acin."
```

Komutu çalıştırdıktan sonra **yeni bir PowerShell penceresi açın** ve tekrar doğrulayın.

### Araçlar bulundu fakat yine de tanınmıyorsa — Kalıcı Çözüm

Araçların PATH'e eklenmiş olmasına rağmen her yeni terminal açıldığında `cmake` / `ninja` / `arm-none-eabi-gcc` tanınmıyorsa, Windows'un kullanıcı PATH değişikliklerini mevcut oturumlara yansıtmaması nedeniyle oluşan bir VS Code terminal sorunudur.

**Adım 1 — Mevcut terminalde hemen aktif et:**

```powershell
$env:PATH = [System.Environment]::GetEnvironmentVariable("PATH","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("PATH","User")
```

Bu komut yalnızca açık terminale etki eder. Kalıcı çözüm için Adım 2'ye geçin.

**Adım 2 — Kalıcı düzeltme (her yeni terminalde otomatik çalışır):**

```powershell
if (!(Test-Path $PROFILE)) { New-Item -ItemType File -Path $PROFILE -Force | Out-Null }
$line = '$env:PATH = [System.Environment]::GetEnvironmentVariable("PATH","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("PATH","User")'
if (!(Get-Content $PROFILE -ErrorAction SilentlyContinue | Select-String -SimpleMatch 'GetEnvironmentVariable')) {
    Add-Content $PROFILE $line
    Write-Host "Profil guncellendi: $PROFILE"
} else {
    Write-Host "Zaten mevcut."
}
```

Bu komut, PowerShell profil dosyasına PATH yenileme satırı ekler. Bundan sonra VS Code'da her yeni terminal açıldığında araçlar otomatik olarak tanınır.

---

## 7. VS Code Eklentileri

VS Code içinde (`Ctrl+Shift+X`) aşağıdaki eklentileri arayıp kurun:

| Eklenti Adı | Yayıncı | Açıklama |
|---|---|---|
| **C/C++** | Microsoft | IntelliSense, hata vurgulama |
| **CMake Tools** | Microsoft | CMake entegrasyonu |
| **Cortex-Debug** | marus25 | Mikrodenetleyici debug desteği |

---

## 8. Projeyi Açma

1. VS Code'u açın.
2. **File → Open Folder** (`Ctrl+K Ctrl+O`) menüsünden proje klasörünü seçin:
   ```
   Activity1_Sensor_Connectivity_and_MQTT\Project_MQTT\AUDK32_A34xxxx-1.0.11\
   ```
3. Sol altta CMake Tools durum çubuğu **"No Kit Selected"** uyarısı verebilir — bu adımı atlayın, preset sistemi kullanıldığı için kit seçimi gerekmez.

---

## 9. Derleme (Build)

VS Code'da **Terminal → New Terminal** ile terminali açın (`Ctrl+`` `).

### Adım 1 — CMake yapılandır

```powershell
cmake --preset debug
```

Bu komut `build/debug/` klasörünü oluşturur ve Ninja build dosyalarını hazırlar.

Başarılı çıktı:
```
-- Configuring done
-- Generating done
-- Build files have been written to: .../build/debug
```

### Adım 2 — Derle

```powershell
cmake --build --preset debug
```

Bu komut tüm `.c` dosyalarını derleyip `build/debug/tmpl_userapp.elf` çıktısını üretir.

Başarılı çıktı (son satırlar):
```
text    data     bss     dec     hex filename
XXXXX    XXX    XXXX   XXXXX   XXXXX tmpl_userapp
```

> **Release derlemek için** `debug` yerine `release` yazın.

---

## 10. Mikrodenetleyiciye Yükleme (Flash)

### Flash komutu

```powershell
cmake --build --preset flash-debug
```

Başarılı çıktı:
```
** Programming Started **
** Programming Finished **
** Verify Started **
** Verified OK **
shutdown command invoked
```

> Kod yüklendikten sonra mikrodenetleyici otomatik olarak çalışmaya başlar.

---

## 11. Tek Komutla Derleme ve Yükleme

Proje kök klasöründeki `flash.bat` dosyasını çalıştırarak yapılandırma, derleme ve yükleme adımlarını tek seferde yapabilirsiniz:

```powershell
.\flash.bat
```

Bu dosya sırasıyla şunları çalıştırır:
1. `cmake --preset debug` — Yapılandır
2. `cmake --build --preset debug` — Derle
3. `cmake --build --preset flash-debug` — Yükle

---

## 12. Proje Yapısı

```
Activity1_Sensor_Connectivity_and_MQTT/
└── Project_MQTT/
    └── AUDK32_A34xxxx-1.0.11/
        ├── CMakeLists.txt          ← Derleme tarifi (kaynak dosyalar, bayraklar, flash)
        ├── CMakePresets.json       ← debug / release / flash-debug presetleri
        ├── openocd.cfg             ← OpenOCD flash ve reset konfigürasyonu
        ├── flash.bat               ← Tek komutla derle + yükle
        ├── cmake/
        │   └── arm-none-eabi.cmake ← ARM cross-compiler toolchain tanımı
        ├── Example/Source/TmplUserApp/
        │   ├── main.c              ← Uygulama giriş noktası
        │   ├── prv_user_code.c     ← Kullanıcı kodu
        │   └── ...                 ← Sensör, MQTT, WiFi kütüphaneleri
        ├── Platform/HAL/           ← Donanım soyutlama katmanı (ADC, I2C, UART vb.)
        ├── Framework/CMSIS/        ← ARM CMSIS başlık dosyaları ve startup kodu
        ├── ProductConfig/          ← Chip konfigürasyon başlıkları
        └── Tool/OpenOCD/           ← OpenOCD flash aracı (dahili, kurulum gerekmez)
```

### Build çıktıları (`build/debug/`)

| Dosya | Açıklama |
|---|---|
| `tmpl_userapp.elf` | Debug sembollerini içeren tam binary (GDB / flash için) |
| `tmpl_userapp.hex` | Intel HEX formatı |
| `tmpl_userapp.bin` | Ham binary |
| `tmpl_userapp.map` | Bellek haritası (Flash/RAM kullanımı) |

---

## 13. Sık Karşılaşılan Hatalar

### `cmake` / `ninja` / `arm-none-eabi-gcc` komutu tanınmıyor

Araçlar kurulu olsa bile PATH'e eklenmemiş olabilir. Bu üç durum sık yaşanır:

- **CMake:** `winget install` sonrası PATH'e otomatik eklenir ancak mevcut terminal oturumunu yenilemeniz gerekir.
- **Ninja:** `winget` ile kurulunca `AppData\Local\Microsoft\WinGet\Packages\...` altına düşer, PATH'e **otomatik eklenmez**.
- **ARM GCC:** Kurulum sihirbazında **"Add path to environment variable"** seçeneği işaretlenmemişse PATH'e girmez.

Çözüm için Bölüm 6'daki PowerShell komutunu çalıştırın. Komut sonrası yeni terminal açmak zorunludur.

### `The current CMakeCache.txt directory ... is different` hatası

Proje klasörü farklı bir konumdan kopyalandıysa ya da taşındıysa eski `build/` klasörü önceki konumu hatırlar. Çözüm:

```powershell
Remove-Item -Recurse -Force build
cmake --preset debug
```

`build/` klasörünü silip yeniden yapılandırmak yeterlidir, kaynak dosyalara dokunulmaz.

### `arm-none-eabi-gcc` bulunamıyor
Toolchain kurulumunda **"Add to PATH"** seçeneği işaretlenmemiştir. Bölüm 6'daki PowerShell komutunu çalıştırarak otomatik olarak ekleyin.

### `ninja: no work to do`
Kaynak dosyalar değişmemiştir. Normaldir, önceki derleme hâlâ geçerlidir.

### `AP write error` / `timed out while waiting for target halted`
Debug kartının bağlantısını kontrol edin. SWDIO, SWCLK ve GND kablolarının takılı olduğundan emin olun. Kartı USB'den çıkarıp yeniden takın.

### `Error: unable to find a matching CMSIS-DAP device`
Sürücü sorunu olabilir. Device Manager'da debug kartının sarı ünlem işareti alıp almadığına bakın. Zadig (https://zadig.akeo.ie) ile WinUSB sürücüsü yükleyin.

### Script çalıştırılamıyor (`.ps1` hatası)
`.bat` uzantılı dosyayı kullanın: `.\flash.bat`

---

## 14. Debug Mesajlarını Görme (Seri Terminal)

Kartüstü `LOG(...)` çıktıları, MCU'nun UART pini üzerinden bilgisayara gönderilir. Bu mesajları okuyabilmek için bir seri terminal gerekmektedir.

### Bağlantı ayarları

| Parametre | Değer |
|---|---|
| Baud Rate | **115200** |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |

### Tera Term

1. [https://teratermproject.github.io](https://teratermproject.github.io) adresinden indirin.
2. Uygulamayı açın, **Serial** bağlantı tipini seçin.
3. **COM portunu** ve **Baud Rate** olarak `115200` girin.
4. **Open / Bağlan** butonuna tıklayın.
5. Kartı flash'layıp çalıştırın; debug mesajları terminal penceresinde görünür.

### COM portunu bulma

**Aygit Yöneticisi** (`Win+X` → Device Manager) → **Ports (COM & LPT)** altında debug kartının COM numarasını görebilirsiniz (genellikle **USB Serial Device** veya **CMSIS-DAP** olarak görünür).

---

## 15. Google Colab ile Çalışma

**Google Colab** (Colaboratory), Google tarafından sunulan, tarayıcı üzerinden Python kodları yazmanıza, çalıştırmanıza ve paylaşmanıza olanak tanıyan ücretsiz, bulut tabanlı bir Jupyter Notebook ortamıdır. Kurulum gerektirmeyen bu platform, özellikle makine öğrenimi, veri bilimi ve yapay zeka araştırmaları için yüksek performanslı GPU ve TPU kaynaklarına ücretsiz erişim sağlar.

### Google Colab'a Giriş

1. Tarayıcınızda şu adrese gidin: [https://colab.research.google.com](https://colab.research.google.com)
2. Sağ üst köşedeki **"Sign in"** butonuna tıklayın.
3. **Google hesabınızla** (Gmail) giriş yapın.
4. Giriş yaptıktan sonra yeni bir notebook oluşturabilir veya mevcut bir notebook'u açabilirsiniz.