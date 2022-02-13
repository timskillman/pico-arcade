add_library(pico_gy521 gy521.cpp gy521.h)

target_link_libraries(pico_gy521 pico_stdlib hardware_i2c)

target_include_directories(pico_gy521 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
