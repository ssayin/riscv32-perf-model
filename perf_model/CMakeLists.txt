add_subdirectory(lib)

add_executable(model src/model/Core.cpp src/model/Model.cpp src/main.cpp)

target_include_directories(model PRIVATE src include)
target_link_libraries(model PRIVATE spdlog visible)

target_include_directories(model PRIVATE ${rapidjson_SOURCE_DIR})
