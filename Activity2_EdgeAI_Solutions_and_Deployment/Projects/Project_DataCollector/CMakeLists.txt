cmake_minimum_required(VERSION 3.20)

# ============================================================================
# ABOV A34G43x - Sensor Gateway Firmware
# Cortex-M4F | Flash: 256KB @ 0x00000000 | RAM: 128KB @ 0x20000000
# Toolchain : arm-none-eabi-gcc
# Build     : Visual Studio / CMake / Ninja
# Flash     : cmake --build build/debug --target flash
# ============================================================================

# Toolchain dosyasini project() cagrilmadan once belirt
set(CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi.cmake"
    CACHE STRING "" FORCE)

project(tmpl_userapp C ASM)

set(TARGET_NAME tmpl_userapp)

# ─── Post-build araclari (objcopy, size) ─────────────────────────────────────
find_program(OBJCOPY arm-none-eabi-objcopy REQUIRED)
find_program(SIZE    arm-none-eabi-size    REQUIRED)

# ─── Linker script ───────────────────────────────────────────────────────────
set(LINKER_SCRIPT
    "${CMAKE_SOURCE_DIR}/Framework/CMSIS/Device/ABOV/Source/GCC/gcc_a34g43x.ld")

# ============================================================================
# KAYNAK DOSYALAR
# ============================================================================

# --- Baslangic / System ---
set(STARTUP_SOURCES
    Framework/CMSIS/Device/ABOV/Source/startup_a34xxxx.c
    Framework/CMSIS/Device/ABOV/Source/system_a34xxxx.c
)

# --- Uygulama ---
set(APP_SOURCES
    Example/Source/TmplUserApp/main.c
    Example/Source/TmplUserApp/prv_scu_clk.c
    Example/Source/TmplUserApp/prv_user_code.c
    Example/Source/TmplUserApp/user_adc.c
    Example/Source/TmplUserApp/user_adc_isr.c
    Example/Source/TmplUserApp/user_i2c.c
    Example/Source/TmplUserApp/user_i2c_isr.c
    Example/Source/TmplUserApp/user_timer1.c
    Example/Source/TmplUserApp/user_timer1_isr.c
    Example/Source/TmplUserApp/user_uart.c
    Example/Source/TmplUserApp/user_uart_isr.c
)

# --- HAL suruculer ---
set(HAL_SOURCES
    Platform/HAL/ADC/hal_adc.c
    Platform/HAL/AES/hal_aes.c
    Platform/HAL/CMP/hal_cmp.c
    Platform/HAL/CRC/hal_crc.c
    Platform/HAL/DAC/hal_dac.c
    Platform/HAL/FMC/hal_fmc.c
    Platform/HAL/FRT/hal_frt.c
    Platform/HAL/HPL/DMA/hpl_dma.c
    Platform/HAL/HPL/NMI/hpl_nmi.c
    Platform/HAL/I2C/hal_i2c.c
    Platform/HAL/LCD/hal_lcd.c
    Platform/HAL/LPUART/hal_lpuart.c
    Platform/HAL/MPWM/hal_mpwm.c
    Platform/HAL/PCU/hal_pcu.c
    Platform/HAL/PGA/hal_pga.c
    Platform/HAL/PWM/hal_pwm.c
    Platform/HAL/QEI/hal_qei.c
    Platform/HAL/QSPI/hal_qspi.c
    Platform/HAL/RNG/hal_rng.c
    Platform/HAL/RTC/hal_rtc.c
    Platform/HAL/SCU/hal_scu.c
    Platform/HAL/SCU/hal_scu_clk.c
    Platform/HAL/SCU/hal_scu_lvd.c
    Platform/HAL/SCU/hal_scu_pwr.c
    Platform/HAL/SPI/hal_spi.c
    Platform/HAL/Timer1/hal_timer1.c
    Platform/HAL/UART/hal_uart.c
    Platform/HAL/USART/hal_usart.c
    Platform/HAL/VREFBUF/hal_vrefbuf.c
    Platform/HAL/WDT/hal_wdt.c
)

# --- Debug kutuphanesi (ABOV) ---
set(DEBUG_LIB_SOURCES
    Platform/Library/ABOV/Debug/Source/debug.c
    Platform/Library/ABOV/Debug/Source/debug_cmd.c
    Platform/Library/ABOV/Debug/Source/debug_hardfault.c
    Platform/Library/ABOV/Debug/Source/debug_log.c
    Platform/Library/ABOV/Debug/Source/debug_retarget.c
    Platform/Library/ABOV/Debug/Source/debug_serial.c
)

# --- Kullanici kutuphaneleri (sensor, wifi, mqtt) ---
set(USER_LIB_SOURCES
    Example/Source/TmplUserApp/DebugLibrary/debug_framework.c

    Example/Source/TmplUserApp/SHT40/SHT40_app.c
    Example/Source/TmplUserApp/SHT40/sht4x_i2c.c
    Example/Source/TmplUserApp/SHT40/sensirion_i2c_hal.c
    Example/Source/TmplUserApp/SHT40/sensirion_common.c
    Example/Source/TmplUserApp/SHT40/sensirion_i2c.c

    Example/Source/TmplUserApp/MP23ABS1/mp23abs1_sensor.c
    Example/Source/TmplUserApp/BatteryReading/battery_reading.c

    Example/Source/TmplUserApp/LISDE12TR/lis2de12.c
    Example/Source/TmplUserApp/LISDE12TR/LIS2DE12_app.c

    Example/Source/TmplUserApp/ESP32_AT_Test/ESP32_AT_Test.c

    Example/Source/TmplUserApp/MQTT_Library/mqtt_port_abov.c
    Example/Source/TmplUserApp/MQTT_Library/mqtt_core.c
    Example/Source/TmplUserApp/MQTT_Library/EMPA_MqttAws.c

    Example/Source/TmplUserApp/Sensor/sensor.c
)

# --------- AI SOURCES STARTS HERE ---------

# --------- AI SOURCES ENDS HERE ---------

# ============================================================================
# EXECUTABLE
# ============================================================================
add_executable(${TARGET_NAME}
    ${STARTUP_SOURCES}
    ${APP_SOURCES}
    ${HAL_SOURCES}
    ${DEBUG_LIB_SOURCES}
    ${USER_LIB_SOURCES}
)

# Output klasoru: build/debug/ veya build/release/
set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# ─── Include yollari ─────────────────────────────────────────────────────────
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Framework/CMSIS/Core/Include
    ${CMAKE_SOURCE_DIR}/Framework/CMSIS/Device/ABOV/Include
    ${CMAKE_SOURCE_DIR}/ProductConfig/Config
    ${CMAKE_SOURCE_DIR}/ProductConfig/A34G43x/Config
    ${CMAKE_SOURCE_DIR}/Platform/HAL/Include
    ${CMAKE_SOURCE_DIR}/Platform/HAL/HPL/Include
    ${CMAKE_SOURCE_DIR}/Platform/Library/ABOV/Debug/Include
    # Uygulama kodlari icin (SHT40/, LISDE12TR/, vb. relative include'lar)
    ${CMAKE_SOURCE_DIR}/Example/Source/TmplUserApp
    # ---------- AI INCLUDE PATH STARTS HERE ---------

    # ---------- AI INCLUDE PATH ENDS HERE ---------
)

# ─── Preprocessor define'lar ─────────────────────────────────────────────────
target_compile_definitions(${TARGET_NAME} PRIVATE
    EXTRN_SUBFAMILY_A34G43x
    EXTRN_ABOV_MODULE_CONFIG
)

# ─── Derleme secenekleri ─────────────────────────────────────────────────────
target_compile_options(${TARGET_NAME} PRIVATE
    -std=gnu11
    -Wall
    -Wno-unused-parameter
    -ffunction-sections   # Her fonksiyonu ayri section'a al (gc-sections icin)
    -fdata-sections       # Her global veriyi ayri section'a al
    -fno-strict-aliasing
    "$<$<CONFIG:Debug>:-O0;-g3>"
    "$<$<CONFIG:Release>:-O2>"
)

# ─── Linker secenekleri ──────────────────────────────────────────────────────
target_link_options(${TARGET_NAME} PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,--gc-sections                                    # Kullanilmayan code/data'yi sil
    -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET_NAME}.map      # Map dosyasi uret
    -Wl,--print-memory-usage                             # Flash/RAM kullanimi yazdir
    --specs=nosys.specs                                  # Bare-metal (OS yok)
    --specs=nano.specs                                   # Kucuk C library (newlib-nano)
    -lc -lm                                              # C ve math kutuphanesi
)

# ─── Post-build: .hex ve .bin uret, boyut yazdir ─────────────────────────────
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${OBJCOPY} -O ihex
            $<TARGET_FILE:${TARGET_NAME}>
            ${CMAKE_BINARY_DIR}/${TARGET_NAME}.hex
    COMMAND ${OBJCOPY} -O binary
            $<TARGET_FILE:${TARGET_NAME}>
            ${CMAKE_BINARY_DIR}/${TARGET_NAME}.bin
    COMMAND ${SIZE} --format=berkeley
            $<TARGET_FILE:${TARGET_NAME}>
    COMMENT ">>> ${TARGET_NAME}.hex / .bin olusturuldu"
    VERBATIM
)

# ─── 'flash' target: OpenOCD ile cipe yukle ──────────────────────────────────
set(OPENOCD "${CMAKE_SOURCE_DIR}/Tool/OpenOCD/win32/openocd.win32.exe")
set(OPENOCD_SCRIPTS "${CMAKE_SOURCE_DIR}/Tool/OpenOCD/share/openocd/scripts")
set(OPENOCD_CFG "${CMAKE_SOURCE_DIR}/openocd.cfg")

add_custom_target(flash
    DEPENDS ${TARGET_NAME}
    COMMAND ${OPENOCD}
            -s "${OPENOCD_SCRIPTS}"
            -f "${OPENOCD_CFG}"
            -c "init"
            -c "halt"
            -c "program {${CMAKE_BINARY_DIR}/${TARGET_NAME}} verify"
            -c "reset run"
            -c "exit"
    COMMENT ">>> OpenOCD ile ${TARGET_NAME}.elf yukleniyor..."
    VERBATIM
)
