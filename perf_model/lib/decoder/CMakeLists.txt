# SPDX-FileCopyrightText: 2022 - 2023 Serdar Sayın <https://serdarsayin.com>
#
# SPDX-License-Identifier: MIT

add_library(riscv32-sim-common INTERFACE)
target_include_directories(riscv32-sim-common INTERFACE include)

add_library(riscv32-decoder OBJECT include/decoder/decoder.hpp
                                   src/decoder16.cpp src/decoder.cpp)

target_include_directories(riscv32-decoder PUBLIC include)
target_link_libraries(riscv32-decoder PUBLIC riscv32-sim-common)

if(COVERAGE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(riscv32-decoder
                           PRIVATE "-fprofile-arcs;-ftest-coverage")
    target_link_libraries(riscv32-decoder PRIVATE "-lgcov")
  endif()
endif()
