add_executable(
  picoarcade
  main.cpp
  Common/Common.h
  games/Uprising/Uprising.cpp
  games/Fire/Fire.cpp
  games/BrickBash/BrickBash.cpp
  games/ScreenSavers/ScreenSaver.cpp
  games/ScreenSavers/StarField.cpp
  games/ScreenSavers/RotoZoom.cpp
  games/ScreenSavers/Plasma.cpp
  games/ScreenSavers/MetaBalls.cpp
  games/ScreenSavers/BouncingBalls.cpp
  games/ScreenSavers/Strings.cpp
  games/ScreenSavers/Triangles.cpp
  games/ScreenSavers/GfxTest.cpp
  games/Asteroids/Asteroids.cpp
  games/Asteroids/ship.cpp
  games/Asteroids/asteroid.cpp
  games/Asteroids/vectorart.cpp
  games/Asteroids/ufo.cpp
  games/Asteroids/missile.cpp
  games/MissileBomber/MissileBomber.cpp
  games/MissileBomber/Missiles.cpp
  games/StarHooper/StarHooper.cpp
)

# Pull in pico libraries that we need
target_link_libraries(picoarcade pico_stdlib hardware_spi hardware_i2c hardware_pwm hardware_dma pico_display pico_widgets pico_mpu_9250 pico_gy521)

# create map/bin/hex file etc.
pico_add_extra_outputs(picoarcade)

# Change the following line to configure whatever Pico Arcade machine you're compiling for...

include(${CMAKE_CURRENT_SOURCE_DIR}/PicoArcadeMiniConfig.txt)

