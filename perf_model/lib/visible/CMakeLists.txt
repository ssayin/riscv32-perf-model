add_library(visible OBJECT visible_extract.cpp visible_ostream.cpp)
target_include_directories(visible PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
