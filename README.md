# BruGenie - "Coffee Anyway You Wish"

<img width="275" alt="image" src="https://github.com/user-attachments/assets/e5be9778-61db-4848-8f4d-f0941952f0f3">

This github is dedicated to a real time embedded systems project to develop a real-time coffee machine which can measure the volume of mugs to ensure the mug is always filled to the correct level and that the milk to coffee ratio isalways the same regardless of cup volume.

## Prerequisites

To use this library there are two sets of requirements software and hardware each of which are listed below.

### Hardware

- 2 x I2C Ultrasond Sensors
- 1 x Temperature Sensor
- 1 x Weight Sensor

### Software

```
apt-get 
```

## Building 
The following commands can be executed to build the exectuables. This requires CMake version 3.07 or greater.


```
cmake .
make
```
This will generate the following executables:

### Running Software Tests

To execute the sofrtware tests run the following command after building.

```
make test
```





