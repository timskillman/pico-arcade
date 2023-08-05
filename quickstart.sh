echo Creating Pico Directory in ~
cd ~
mkdir pico
cd pico
echo Installing the Pico SDK...
export PICO_SDK_PATH=`pwd`
cd pico-sdk
git submodule update --init
cd ..
git clone -b master https://github.com/raspberrypi/pico-examples.git

echo Installing C++ Toolchain...
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential

echo Installing Pico Arcade and libraries...
git clone https://github.com/timskillman/pico-arcade.git
cd pico-arcade

echo Creating .uf2 file...
mkdir build
cd build
cmake ..
make
cd ~/pico/
