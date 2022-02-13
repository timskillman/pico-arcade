add_library(st7735
    ${CMAKE_CURRENT_LIST_DIR}/st7735.cpp)

target_include_directories(st7735 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(st7735 pico_stdlib hardware_spi hardware_pwm hardware_dma)
