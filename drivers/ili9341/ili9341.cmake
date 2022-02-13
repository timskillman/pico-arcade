add_library(ili9341
    ${CMAKE_CURRENT_LIST_DIR}/ili9341.cpp)

target_include_directories(ili9341 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(ili9341 pico_stdlib hardware_spi hardware_pwm hardware_dma)
