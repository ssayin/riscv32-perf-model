add_executable(visible_test visible_test.cpp ../visible/visible_extract.cpp
                            ../visible/visible_ostream.cpp)

target_include_directories(visible_test PUBLIC ../visible)
