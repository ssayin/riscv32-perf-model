add_library(visible OBJECT src/visible_extract.cpp src/visible_ostream.cpp)

target_include_directories(visible PUBLIC src)

add_executable(visible_test src/visible_test.cpp)
target_link_libraries(visible_test PUBLIC visible)
