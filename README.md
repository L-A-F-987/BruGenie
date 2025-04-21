# BruGenie - "Coffee Any Way You Wish"

<img width="400" alt="image" src="Images/BruGenie Black + White.svg">

This GitHub repository showcases a real-time embedded systems project focused on developing a smart coffee machine. The machine measures mug volume to ensure each cup is filled to the perfect level, maintaining a consistent milk-to-coffee ratio regardless of cup size.

## Table of Contents

- [Bill of Materials](#bill-of-materials)
- [Building](#building)
- [Social Media Channels](#social-media-channels)
- [Acknowledgements](#acknowledgements)
- [Authors & Contributions](#authors-and-contributions)

## Bill of Materials

### Controller

| Microcontroller  | Quantity | Cost (£) |
|------------------|----------|----------|
| Raspberry Pi 5   | 1        |     58.98     |

### Sensor

| Sensors                                                        | Quantity | Cost (£) |
|----------------------------------------------------------------|----------|----------|
| VL53L4CD – Time-of-Flight Sensor                               | 2        |  33.00        |
| DS18B20 – Temperature Sensor                                   | 1        |     4.20     |
| Parallax Inc Servo Motor (4–6V, 50 RPM, 27 N·cm Max Torque)    | 1        |    20.00      |

### Supporting/Miscellaneous Components

| Additional Components                                    | Quantity | Cost (£) |
|----------------------------------------------------------|----------|----------|
| Solenoid Valve ST-SA Series (Normally Closed)            | 2        |     85.86     |
| AC/DC Power Supply                                       | 1        |    6.67      |
| 10mm Lead Screw                                          | 1        |      31.82    |
| Igus Flanged Round Nut – 10mm                            | 1        |     48.61     |
| Liquid Reservoir                                         | 1        |      N/A    |
| Microswitch                                              | 1        |     0.80     |

**Grand Total:** £289.94

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

## User Case UML Diagram

This sequence diagram shows the steps taken by the coffee machine system after a user places a mug on the platform.

The process begins when the user places a mug, triggering the microswitch. The microswitch activates the servo motor, which starts the volume computation process. The volume computation component communicates with the time-of-flight sensor to boot it, start recording data, and stop recording once the volume is measured.

After receiving the volume data, the volume computation component sends a command to the solenoid controller to open the solenoid valve and pour the liquid. Once the required amount is dispensed, the solenoid is closed. Finally, the system informs the user that the coffee is ready.

![UMLBruGenie](https://github.com/user-attachments/assets/4680fc1d-73ef-4f58-8192-39eba4e9d994)


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

We would like to thank Bernd Porr for providing us with the opportunity to work on this project and for supervising us throughout. We also extend our gratitude to RS Components for funding the project, their support played a key role in bringing our idea to life. Finally, a haertfelt thank you to the mechanical workshop, as well as the electronics and printing lab technicians, for their help in streamlining the build and completion of the system.

## Authors and Contributions

- **Luca Faccenda** - Completed the ADS1115 (weight sensor) class, Solenoid Controller, and VL53L4CD (time-of-flight) driver by translating it from C to C++. Also worked on volume control, motor control, and the pin control wrapper.
- **Manaf Habeeb** - Completed CAD drawings and models, 3D printing, unit testing, and social media outreach.
- **Tanvi Kapil** - Completed the ADS1115 (weight sensor) class, Solenoid Controller, and the VL53L4CD (time-of-flight) driver by translating it from C to C++. Also worked on volume control and motor control.
- **Taniya Jose** - Completed CAD drawings and models, 3D printing, Raspberry Pi setup, and wrote the RS DesignSpark article for the student fund. 
- **Maarven Pathmanabhan** - Completed all circuit design and wiring, and worked on the DS18B20 temperature sensor.

## Contact Us

Company Email: c0ffee.embedded@gmail.com

## Last Updated
This README was last updated on 21/04/2025.
