include(${CMAKE_CURRENT_LIST_DIR}/../pico_display/pico_display.cmake)

add_library(pico_widgets INTERFACE)

target_sources(pico_widgets INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/pico_widgets.cpp
)

target_include_directories(pico_widgets INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(pico_widgets INTERFACE pico_stdlib pico_display)
