add_library(gy521
    ${CMAKE_CURRENT_LIST_DIR}/gy521.cpp)

target_include_directories(gy521 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(gy521 pico_stdlib hardware_spi hardware_pwm hardware_dma hardware_i2c)
