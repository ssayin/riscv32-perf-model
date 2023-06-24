add_subdirectory(lib)

add_executable(model src/main.cpp)

target_include_directories(model PRIVATE include)
target_link_libraries(model PRIVATE spdlog riscv32-decoder)

target_include_directories(model PRIVATE ${json_SOURCE_DIR}/include)
