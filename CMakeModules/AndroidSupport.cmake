#
#  CMake custom modules
#  Copyright (C) 2011-2015  Cedric OCHS
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

MACRO(INIT_BUILD_FLAGS_ANDROID)
  IF(ANDROID)
    ADD_PLATFORM_FLAGS("--sysroot=${PLATFORM_ROOT}")
    ADD_PLATFORM_FLAGS("-ffunction-sections -funwind-tables -no-canonical-prefixes")
    ADD_PLATFORM_FLAGS("-DANDROID")
    ADD_PLATFORM_FLAGS("-I${STL_INCLUDE_DIR} -I${STL_INCLUDE_CPU_DIR}")

    ADD_PLATFORM_FLAGS("-fstack-protector-strong")
    ADD_PLATFORM_FLAGS("-Wa,--noexecstack")

    IF(CLANG)
      IF(TARGET_ARM64)
        SET(LLVM_TRIPLE "aarch64-none-linux-android")
      ELSEIF(TARGET_ARMV7)
        SET(LLVM_TRIPLE "armv7-none-linux-androideabi")
      ELSEIF(TARGET_ARMV5)
        SET(LLVM_TRIPLE "armv5te-none-linux-androideabi")
      ELSEIF(TARGET_X64)
        SET(LLVM_TRIPLE "x86_64-none-linux-android")
      ELSEIF(TARGET_X86)
        SET(LLVM_TRIPLE "i686-none-linux-android")
      ELSEIF(TARGET_MIPS64)
        SET(LLVM_TRIPLE "mips64el-none-linux-android")
      ELSEIF(TARGET_MIPS)
        SET(LLVM_TRIPLE "mipsel-none-linux-android")
      ELSE()
        MESSAGE(FATAL_ERROR "Unspported architecture ${TARGET_CPU}")
      ENDIF()

      ADD_PLATFORM_FLAGS("-gcc-toolchain ${GCC_TOOLCHAIN_ROOT}")
      ADD_PLATFORM_LINKFLAGS("-gcc-toolchain ${GCC_TOOLCHAIN_ROOT}")

      ADD_PLATFORM_FLAGS("-target ${LLVM_TRIPLE}") # -emit-llvm -fPIC ?
      ADD_PLATFORM_LINKFLAGS("-target ${LLVM_TRIPLE}")

      # better debugability with LLDB
      SET(DEBUG_CFLAGS "${DEBUG_CFLAGS} -fno-limit-debug-info")
    ENDIF()

    IF(TARGET_ARM)
      ADD_PLATFORM_FLAGS("-fpic")
      ADD_PLATFORM_FLAGS("-D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__")

      IF(TARGET_ARM64)
        # no specific options
      ELSEIF(TARGET_ARMV7)
        # only correct archs are managed
        ADD_PLATFORM_FLAGS("-march=armv7-a -mfpu=vfpv3-d16")
        ADD_PLATFORM_FLAGS("-mfloat-abi=softfp")

        IF(CLANG)
          ADD_PLATFORM_FLAGS("-fno-integrated-as")
        ENDIF()

        ADD_PLATFORM_LINKFLAGS("-Wl,--fix-cortex-a8")
      ELSEIF(TARGET_ARMV5)
        IF(CLANG)
          ADD_PLATFORM_FLAGS("-fno-integrated-as")
        ENDIF()

        ADD_PLATFORM_FLAGS("-march=armv5te -mtune=xscale -msoft-float")
      ENDIF()

      SET(TARGET_THUMB ON)

      IF(TARGET_THUMB)
        ADD_PLATFORM_FLAGS("-mthumb")
      ELSE()
        ADD_PLATFORM_FLAGS("-marm")
      ENDIF()
    ELSEIF(TARGET_X86)
      # Same options for x86 and x86_64
      ADD_PLATFORM_FLAGS("-fPIC")
    ELSEIF(TARGET_MIPS)
      IF(NOT TARGET_MIPS64)
        # We need to specify that to force an older arch for compatibility
        ADD_PLATFORM_FLAGS("-mips32")
        ADD_PLATFORM_LINKFLAGS("-mips32")
      ENDIF()

      ADD_PLATFORM_FLAGS("-fpic -finline-functions -fmessage-length=0")
    ENDIF()

    ADD_PLATFORM_LINKFLAGS("-Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -no-canonical-prefixes")
    ADD_PLATFORM_LINKFLAGS("-L${PLATFORM_ROOT}/usr/lib")
  ENDIF()
ENDMACRO()
