# Pico Arcade Games & Libraries

![alt text](https://github.com/timskillman/pico-arcade/blob/main/images/PicoArcadeMini.jpg "Pico Arcade games")

**Pico Arcade** is written for the Raspberry Pi Pico and is a derivative of the excellent [Pimoroni Pico library](https://github.com/pimoroni/pimoroni-pico/).  
However, many of the drivers and libraries have had a complete overhaul in terms of supporting several new screens and sensors.  

Furthermore, the graphics library is quite a bit faster due to using fixed-point arithmetic throughout.  There are several new features such as;

- gradient fills
- bezier curves
- Faster triangle rendering

Lasty, several games are included to kickstart your game writing experience;

- Breakout
- Uprising - 2.5D rendered game
- Screensavers (Numerous optimised screensavers)
- Missile Bomber
- Asteroids
- Roller ball (Using a GY271 sensor)

All the ibraries and games are written in C++ as Python is typically too slow.


# How to install and compile the picoarcade.uf2

**1. Create a directory**

```bash
cd ~
mkdir pico
cd pico
```

**2. Install the Pico SDK**

```bash
git clone -b master https://github.com/raspberrypi/pico-sdk.git

# Set the PICO_SDK_PATH environment variable to where you just cloned the repo.
export PICO_SDK_PATH=/path/to/pico-sdk

cd pico-sdk
git submodule update --init
cd ..
git clone -b master https://github.com/raspberrypi/pico-examples.git
```
If the Pico SDK was cloned into your pico directory on a Raspberry Pi, then the path environment *might* look like:

```bash
export PICO_SDK_PATH=/home/pi/pico/pico-sdk
```

**3. Install the C++ toolchain needed to build Pico projects** (if not already installed)

**Debian Linux**
```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential
```

**4. Install Pico Arcade and libraries**

```bash
git clone https://github.com/timskillman/pico-arcade.git
cd pico-arcade
```

**5. Create the PicoArcade UF2 file**

```bash
mkdir build
cd build
cmake ..
make
```

The 'picoarcade.uf2' file can be found in the 'games' folder


# How to change screen type and pin configurations

Configuration settings are found in '[games/CMakeLists.txt](games/CMakeLists.txt)':

**1. Display driver configuration**

```bash
  DISPLAY_ROTATION=0   #or, 90,180,270

  DISPLAY_DRIVER_ST7735_160_128
  DISPLAY_DRIVER_ST7735_128_128
  DISPLAY_DRIVER_ST7735_160_80
  DISPLAY_DRIVER_ST7789_240_240
  DISPLAY_DRIVER_ILI9341_320_240
```

**2. Display pins**

```bash
  DISPLAY_CS=5   #Chip Select (CS pin)
  DISPLAY_DC=7   #DC pin
  DISPLAY_SCK=2  #Clock pin
  DISPLAY_MOSI=3 #MOSI (or SDA) pin
  DISPLAY_BL=6   #Back Light pin (or -1 if not used)
  DISPLAY_RST=4  #Reset pin
```

**3. Button pins**

```bash
  BUTTON_A=11   #Button A
  BUTTON_B=21   #Button B
  BUTTON_X=13   #Button X
  BUTTON_Y=12   #Button Y

  BUTTON_L=14   #Mini joystick Left
  BUTTON_R=19   #Mini joystick Right
  BUTTON_U=18   #Mini joystick Up
  BUTTON_D=20   #Mini joystick Down
  BUTTON_F=9    #Mini joystick Fire (middle click)
```

**4. Games to compile**
Note: Including ALL games will compile but over-run on Pico memory (resulting in blank screen!)

```bash
  #GAME_ROLLERBALL     # A rolling ball test routine for the GY521 accelerometer 
  GAME_BREAKOUT        # Classic breakout
  GAME_SCREENSAVERS    # Numerous screen savers that have been optimised for speed
  GAME_UPRISING        # A demo 2.5D game
  GAME_ASTEROIDS       # Classic arcade Asteroids
  #GAME_MISSILEBOMBER  # Classic arcade Missile Command
```

**5. Adding your own games/tests**

'main.cpp' (games folder) sets up and controls which games are run

See [Writing you own arcade game](writing_your_own_game.md) for more information.


# Software support for Devices

The following screens are supported:

- ST7735 (various resolutions)
- ST7739 (various resolutions)
- ili9341

Note: It's very simple to add new screens of the type above although they need to support Chip Select (CS) for now.  It's also simple to orientate the screen in any 90 degree direction.


**Supported devices:**

- GY521 (i2c) - gyro and accelerometer
- mpu_9250 (SPI) - gyro and accelerometer

**Further support coming for**

- DS1302 RTC module (i2c)
- HC05 Bluetooth LE 4.0 (i2c)
- 16 channel Multiplexer (CD74HC4067)
- GPS (GY-GPS6MV2)
- Heart-rate monitor
- PAM8302A audio amp

**Sensors & mechanical:**

- Ultrasonic sensors
- Stepper motor
- Servo motor
- PIR sensor


# Graphics Library improvements 
(over the Pimoroni graphics library)

The graphics library has the following improvements;

- Faster triangle rendering
- Quadratic curves
- Gradient-filled rectangles (multiple colours can be added and evenly spread through the fill)
- General flood fill (can be quite expensive on memory!)
- Textured triangle (not tested!)


# Why create such a library?

This library aims to push beyond basic testing and experimentation to hopefully produce serious prototypes of hand-held devices, mini-game consoles, medical devices and so much more.

Much of the heartache in configuring a project is also removed by using an Arduino style configuration file that enables/disables certain features and configurations.

It enables the use of low-cost components that are typically available from Pimoroni, eBay, Alibaba and the likes.

# Coming Soon!

Pico Arcade is currently under development although there are several developments soon to appear in this repo:

1. A number of exciting software and hardware projects using various sensors. 

2. A cross-platform Pico emulator for faster development

3. Introduce a compact, 'connector free' means of prototyping super-slim devices; easy to wire together and solder using a unique printed circuit board produced on a 3D printer and no special filaments ...

![alt text](https://github.com/timskillman/pico-arcade/blob/main/images/pico-arcade-game2.jpg "Pico Arcade game with 3D printed circuit board")

![alt text](https://github.com/timskillman/pico-arcade/blob/main/images/pico-arcade-game3.jpg "Slim Pico Arcade game in case")


**Watch this space!**




 
