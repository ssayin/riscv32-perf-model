add_executable(visible_test visible_test.cpp)
target_link_libraries(visible_test PUBLIC visible)

add_executable(perf_model perf_model.cpp)
target_link_libraries(perf_model PUBLIC model visible spdlog)
