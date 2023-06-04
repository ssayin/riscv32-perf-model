add_executable(model main.cpp)

target_link_libraries(model PRIVATE spdlog riscv32-decoder)

target_include_directories(model PRIVATE ${json_SOURCE_DIR}/include)
