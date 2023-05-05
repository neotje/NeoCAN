## DEV setup

Install requirements

```
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib build-essential gdb-multiarch doxygen
```

Install openocd
```
$ cd ~/pico
$ sudo apt install automake autoconf texinfo libtool libftdi-dev libusb-1.0-0-dev
$ git clone https://github.com/raspberrypi/openocd.git --branch rp2040 --recursive --depth=1
$ cd openocd
$ ./bootstrap
$ ./configure --enable-ftdi --enable-sysfsgpio --enable-bcm2835gpio
$ make -j4
$ sudo make install
```

Clone sdk to ~/pico/pico-sdk

```
cd ~/pico
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
```

```
git submodule update --init
mkdir build
cd build
cmake ..
```