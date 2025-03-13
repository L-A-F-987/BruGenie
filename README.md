# BruGenie - "Coffee Anyway You Wish"

<img width="400" alt="image" src="Images/Bru Genie Logos + Margins-03.svg">

This GitHub repository showcases a real-time embedded systems project focused on developing a smart coffee machine. The machine measures mug volume to ensure each cup is filled to the perfect level, maintaining a consistent milk-to-coffee ratio regardless of cup size.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building](#building)
- [Social Media Channels](#social-media-channels)
- [Acknowledgements](#acknowledgements)
- [Authors](#authors)

## Prerequisites
### Microcontroller

- Rasberry PI 5

### Sensors

- 2 × VL53L4CD – Time-of-Flight Sensors
- 1 × DS18B20 – Temperature Sensor
- 1 × TE Connectivity – Load Cell
- 1 × ADS1115 – 16-Bit ADC
- 1 × Parallax Inc Servo Motor – 4–6V, 50 RPM, 27 N·cm Max Output Torque

### Additional Components

- 1 × PTC Heating Element
- 1 × 10mm Lead Screw
- 1 × Igus Flanged Round Nut – 10mm
- 1 × Russell Hobbs Coffee Machine

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
This will generate the following executables:

### Running Software Tests

To execute the sofrtware tests run the following command after building.

```
make test
```
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
This README was last updated on 13/03/2025.
