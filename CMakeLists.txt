# SPDX-FileCopyrightText: 2022 - 2023 Serdar Sayın <https://serdarsayin.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.23.2)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(${CMAKE_VERSION} VERSION_GREATER 3.24)
  cmake_policy(SET CMP0135 NEW)
endif()

project(riscv32-perf-model LANGUAGES CXX C ASM)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(FetchContent)
  include(CTest)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

  set(RISCV32_SIM_TRACE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/log)
  file(MAKE_DIRECTORY ${RISCV32_SIM_TRACE_OUTPUT_DIRECTORY})

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  FetchContent_Declare(
    spdlog URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.zip)
  FetchContent_MakeAvailable(spdlog)

  FetchContent_GetProperties(spdlog)
  if(NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    add_library(spdlog INTERFACE)
    target_include_directories(spdlog INTERFACE ${spdlog_SOURCE_DIR}/include)
  endif()

  set(rapidjson_SOURCE_DIR third_party/rapidjson)

  add_subdirectory(perf_model)

endif()
