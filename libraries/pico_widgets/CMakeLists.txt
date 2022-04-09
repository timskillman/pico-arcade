add_library(pico_widgets INTERFACE)

target_sources(pico_widgets INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/pico_widgets.cpp
)

target_include_directories(pico_widgets INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(pico_widgets INTERFACE pico_stdlib pico_graphics)