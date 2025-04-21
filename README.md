# BruGenie - "Coffee Any Way You Wish"

<img width="400" alt="image" src="Images/BruGenie Black + White.svg">

This GitHub repository showcases a real-time embedded systems project focused on developing a smart coffee machine. The machine measures mug volume to ensure each cup is filled to the perfect level, maintaining a consistent milk-to-coffee ratio regardless of cup size.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building](#building)
- [Social Media Channels](#social-media-channels)
- [Acknowledgements](#acknowledgements)
- [Authors](#authors)

## Prerequisites
### Microcontroller

- Raspberry PI 5
  
### Sensors

- 2 × VL53L4CD – Time-of-Flight Sensors
- 1 × DS18B20 – Temperature Sensor
- 1 × Parallax Inc Servo Motor – 4–6V, 50 RPM, 27 N·cm Max Output Torque

### Additional Components
- 2 × Solenoid Valve ST-SA Series Normally Closed
- 1 x AC/DC Power Supply
- 1 × 10mm Lead Screw
- 1 × Igus Flanged Round Nut – 10mm
- 1 × Liquid Reservoir
- 1 × Microswitch


### Libraries

```
apt-get install cmake
```

## Building 
The following commands can be executed to build the exectuables. This requires CMake version 3.07 or greater.

```
cmake .
make
```
This will create executables stored in the following folders for testing of each sensor: 

- /src/VL53L4CD  (TOF Sensor)
- /src/DS18B20_TempSensor (Temperature Sensor)
- /src/Solenoid_Controller (Solenoid Control)
- /src/motor (Servo Motor Control)
- /src/Volume_Comms (Volume Measurement)
- /src/Control_pins (Pin control)

### Running Software Tests

To execute the software tests run the following command after building.

```
make test
```



## Circuit Diagram

This circuit connects two VL53L4CD Time-of-Flight (TOF) sensors, a servo motor, and two solenoids to a Raspberry Pi. The TOF sensors share I2C lines (SDA/SCL) but use separate XSHUT GPIOs to assign unique addresses. The servo is controlled via PWM (GPIO 12), and each solenoid is activated by a GPIO pin through a transistor switch, powered by a 12V source with flyback diodes for protection. The Raspberry Pi acts as the central controller for sensing and actuation.

<img width="428" alt="image" src="https://github.com/user-attachments/assets/f38a16f3-f1e3-4bfd-9b36-1063663790eb" />



## Publication

RS Design Spark article published on 20/03/2025 regarding project purpose and function 
- [Article](https://www.rs-online.com/designspark/brugenie-coffee-anyway-you-wish)


## Social Media Channels

- [Instagram](https://www.instagram.com/brugenie/?locale=en_GB&hl=en)
- [Tiktok](https://www.tiktok.com/@brugenie)
- [Youtube](https://www.youtube.com/channel/UCvZ_YEZ7hOzSlOggeiQ_5xw) 

## Acknowledgements

Thank you to Bernd Porr for providing us with the opportunity to work on this project and for supervising us throughout. We’d also like to extend our gratitude to RS Components for funding our project — their contribution helped bring our idea to life.

## Authors 

- Luca Faccenda
- Manaf Habeeb 
- Tanvi Kapil
- Taniya Jose
- Maarven Pathmanabhan 

## Contact Us

Company Email: c0ffee.embedded@gmail.com

## Last Updated
This README was last updated on 21/04/2025.
