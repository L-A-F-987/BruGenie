# STMICROELECTRONICS - VL53L4CD Linux driver

Official VL53L4CD Linux driver and test applications for linux and android platforms

## Introduction

The proposed implementation is customized to run on a raspberry pi v3, but can be adapted to run on any linux embedded platform,
as far as the VL53L4CD device is connected through I2C
Two options are offered to the user

- 1. compile and run this driver with a kernel module responsible for handling i2c bus and the interruption. This is the kernel mode
- 2. compile and run this driver in a full user mode, where the i2c commnication is handled with the /dev/i2c-1 file descriptor. This is the user mode

Option 1 supports the interruption line of the VL53L4CD but needs a kernel module to be compiled and inserted. Please connect the interrupt pin to GPIO 26 of raspberrypi
Option 2 may be more suitable for simple application, but needs the /dev/i2c-1 to be available which may not be the case on some secured platforms

## How to run a test application on raspberry pi

    Note that the following instructions were tested on raspberrypi 3.

### Install the raspberry pi kernel source headers (kernel mode only)

    refer to raspberrypi official documentation to download the headers matching your kernel version
    $ sudo apt-get install raspberrypi-kernel-headers

### update /boot/config.txt file (kernel mode only)

    $ sudo nano /boot/config.txt
    --> add or uncomment the following lines at the end of the /boot/config.txt
    dtparam=i2c_arm=on
    dtparam=i2c1=on
    dtparam=i2c1_baudrate=100000
    dtoverlay=stmvl53l4cd

### compile the device tree blob (kernel mode only)

    $ cd VL53L4CD_MassMarket_Linux/kernel
    $ make dtb
    $ sudo reboot

### compile the test examples, the platform adaptation layer and the uld driver

    $ nano VL53L4CD_MassMarket_Linux/user/test/Makefile
    Enable or disable the STMVL53L4CD_KERNEL cflags option depending on the wished uld driver mode : with a kernel module of fully in user.
    $ cd VL53L4CD_MassMarket_Linux/user/test
    $ make

### compile the kernel module (kernel mode only)

    $ cd VL53L4CD_MassMarket_Linux/kernel
    $ make clean
    $ make
    $ make insert

### run the test application menu

    $ cd vl53l4cd-uld-driver/user/test
    $ sudo ./menu
