add_executable(
  picoarcade
  main.cpp
  Common/Common.h
  games/VectorMap/Vector.cpp
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
  games/Asteroids/Asteroids.cpp
  games/Asteroids/ship.cpp
  games/Asteroids/asteroid.cpp
  games/Asteroids/vectorart.cpp
  games/Asteroids/ufo.cpp
  games/Asteroids/missile.cpp
  games/MissileBomber/MissileBomber.cpp
  games/MissileBomber/Missiles.cpp
  games/RollerBall/RollerBall.cpp
)

# Pull in pico libraries that we need
target_link_libraries(picoarcade pico_stdlib hardware_spi hardware_i2c hardware_pwm hardware_dma pico_display pico_mpu_9250 pico_gy521)

# create map/bin/hex file etc.
pico_add_extra_outputs(picoarcade)

target_compile_definitions(picoarcade PRIVATE

##### DISPLAY ROTATION (options = 0,90,180,270) ################################

  DISPLAY_ROTATION=0 	

##### DISPLAY DRIVER ###########################################################

  #DISPLAY_DRIVER_ST7735_160_128
  #DISPLAY_DRIVER_ST7735_128_128
  #DISPLAY_DRIVER_ST7735_160_80
  #DISPLAY_DRIVER_ST7789_240_240
  DISPLAY_DRIVER_ILI9341_320_240

##### DISPLAY PINS TO PICO #####################################################

  #3DPCB PINS
  DISPLAY_CS=5  #17
  DISPLAY_DC=7  #20
  DISPLAY_SCK=2 #18
  DISPLAY_MOSI=3  #19
  DISPLAY_BL=6  #-1
  DISPLAY_RST=4   #16

  #BOTTOM RIGHT PINS OF PICO
  #DISPLAY_CS=17
  #DISPLAY_DC=20
  #DISPLAY_SCK=18
  #DISPLAY_MOSI=19
  #DISPLAY_BL=-1
  #DISPLAY_RST=16

  #PICO ARCADE (TOP LEFT PINS OF PICO)
  #DISPLAY_CS=1
  #DISPLAY_DC=0
  #DISPLAY_SCK=2
  #DISPLAY_MOSI=3
  #DISPLAY_BL=-1
  #DISPLAY_RST=4

##### BUTTON PINS ##############################################################
  BUTTON_A=11
  BUTTON_B=21
  BUTTON_X=13
  BUTTON_Y=12

  BUTTON_L=14
  BUTTON_R=19
  BUTTON_U=18
  BUTTON_D=20
  BUTTON_F=9

##### GAMES ####################################################################

  #GAME_ROLLERBALL
  GAME_BREAKOUT
  GAME_SCREENSAVERS
  GAME_UPRISING
  GAME_ASTEROIDS
  #GAME_MISSILEBOMBER

################################################################################

)
