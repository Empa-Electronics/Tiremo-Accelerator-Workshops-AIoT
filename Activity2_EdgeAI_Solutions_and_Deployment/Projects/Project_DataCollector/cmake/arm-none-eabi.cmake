# ============================================================================
# ARM GCC Cross-Compiler Toolchain File
# Target: ABOV A34G43x (Cortex-M4F)
#
# KURULUM:
#   winget install Arm.GnuArmEmbeddedToolchain
#   veya: https://developer.arm.com/downloads/-/gnu-rm
#   Kurulumdan sonra PATH'e eklendigini kontrol et:
#     arm-none-eabi-gcc --version
# ============================================================================

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain prefix - PATH'te olmasi lazim
set(TOOLCHAIN arm-none-eabi)

# Compiler / linker araclari
set(CMAKE_C_COMPILER   ${TOOLCHAIN}-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER ${TOOLCHAIN}-gcc${CMAKE_EXECUTABLE_SUFFIX})

# Linkeri host toolchain yerine cross compiler bilebilsin
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# CPU flags: Cortex-M4 + FPU (fpv4-sp-d16, hard float ABI)
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS_INIT             "${MCU_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT           "${MCU_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT           "${MCU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT    "${MCU_FLAGS}")
