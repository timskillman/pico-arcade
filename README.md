# Pico Arcade Games & Libraries

Pico Arcade is a derivative of the excellent Pimoroni Pico library and examples.  However, many of the drivers and libraries have had a complete overhaul in terms of supporting several new screens and sensors.  

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

All the ibraries and games are written in C++ as Python is typically too slow!


#How to compile the picoarcade.uf2

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
cd build
cmake ..
make
```

The UF2 file can be found in 'games' folder



# Software support for Devices

The following screens are supported:

- ST7735 (various resolutions)
- ST7739 (various resolutions)
- ili9341

Note: It's very simple to add new screens of the type above although they need to support Chip Select (CS) for now.  It's also simple to orientate the screen in any 90 degree direction.


Supported devices:

- GY271 (i2c) - gyro and accelerometer
- mpu_9250 (SPI) - gyro and accelerometer

Further support coming for

- DS1302 RTC module (i2c)
- HC05 Bluetooth LE 4.0 (i2c)
- 16 channel Multiplexer (CD74HC4067)
- GPS
- Heart-rate monitor
- PAM8302A audio amp

Sensors & mechanical:

- Ultrasonic sensors
- Stepper motor
- Servo motor
- PIR sensor

#Why create such a library?

This library aims to push beyond basic testing and experimentation to hopefully produce serious prototypes of hand-held devices, mini-game consoles, medical devices and so much more.

Much of the heartache in configuring a project is also removed by using an Arduino style configuration file that enables/disables certain features and configurations.

The graphics library is also much faster acheived by removing floating point operations that are considerebly slower when number crunching graphics.
 
