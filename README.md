<<<<<<< HEAD
# BruGenie "Coffee any way you wish" 

<img width="557" alt="image" src="https://github.com/L-A-F-987/BruGenie/blob/main/BruGenie_logo_round.jpg">

Project to develop a coffee machine which can fill any mug to the perfect level every time.

- Measures the coffee cup volume in real time using two mobile ultrasound sensors
- Dispenses both coffee and milk in the a ratio provided by the user calculated for each mug

# Building and Make Files

## This project is designed to allow for building using cmake (min version 3.07). Following the below instructions will allow for the build.

- cd file_location
- cmake . 
- make

### This will create x folders each containing executables:

- x
- y

### Additionally to this there are a series of software tests which can be ran using the following command:

- make test 

# Contact Details 

- Company Email: c0ffee.embedded@gmail.com


 
=======
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



>>>>>>> 4d75bfacf6045a609200755a58b0959ba36ff226


