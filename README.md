# BruGenie - "Coffee Any Way You Wish"

<img width="400" alt="image" src="Images/BruGenie Black + White.svg">

This GitHub repository showcases a real-time embedded systems project focused on developing a smart coffee machine. The machine measures mug volume to ensure each cup is filled to the perfect level, maintaining a consistent milk-to-coffee ratio regardless of cup size.

<img src="https://github.com/user-attachments/assets/8f8290e2-a44a-4a57-bbb8-144e09ec18e9" width="320">

<img src="https://github.com/user-attachments/assets/66c979bc-5e96-45b1-8562-b45a0b5766f4" width="320">

<img src="https://github.com/user-attachments/assets/7c05620b-be04-4458-b716-e5892ed022bd" width="320">

<img src="https://github.com/user-attachments/assets/96a89001-aa37-4dee-81bb-6b1ea816a354" width="320">

<img src="https://github.com/user-attachments/assets/103f9ef5-c6cb-4238-85c1-f80a52890cb7" width="380">


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

### Cloning
To clone with all submodules, run:
```
git clone --recursive
```
If the submodules do not download, run:

```
git submodule update --recursive
```

### Libraries

```
apt-get install cmake
sudo apt install libgpiod-dev
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

To execute the software tests run the following command after building, note that these tests only run basic tests for the sensors, this is because each of the other wrappers involve writing directly to set GPIO pin states. Test executables for each of these can therefore be found in each subdirectory for their respective class.

```
make test
```

## Classes Within this Gihub 

### /src/VL53L4CD 

This github contains a c++ interrupt based driver for the VL53L4CD TOF sensor. This class can be used to read values from the VL53L4CD sensor/s and allows for a threaded callback which can be overwritten for the desired application.

### /src/motor

This github contains a wrapper for a the parallax motor mentioned in the bill of materials, this wrapper uses a pre existing PWM class with the correct frequency and cycle to allow for the motor to be controlled.

### /src/Control_Pins

This github contains a wrapper for the gpiod class to allow for simple writing to gpio pins on the Raspberry Pi 5.

### /src/Solenoid_Controller

This github contains a wrapper for the control pins class that manages the two solennoids through the use of the transistors shown. 

### /src/DS18b20

This github contains a class for regularly sampling the DS18b20 temperature sensor on a one wire protocol using an existing cpp_timer class. This can be adapted easily to be used for the desired sampling interval.


## User Case UML Diagram

This sequence diagram shows the steps taken by the coffee machine system after a user places a mug on the platform.

The process begins when the user places a mug, triggering the microswitch. The microswitch activates the servo motor, which starts the volume computation process. The volume computation component communicates with the time-of-flight sensor to boot it, start recording data, and stop recording once the volume is measured.

After receiving the volume data, the volume computation component sends a command to the solenoid controller to open the solenoid valve and pour the liquid. Once the required amount is dispensed, the solenoid is closed. Finally, the system informs the user that the coffee is ready.

![UMLBruGenie](https://github.com/user-attachments/assets/4680fc1d-73ef-4f58-8192-39eba4e9d994)


## Circuit Diagram

This circuit connects two VL53L4CD Time-of-Flight (TOF) sensors, a servo motor, and two solenoids to a Raspberry Pi. The TOF sensors share I2C lines (SDA/SCL) but use separate XSHUT GPIOs to assign unique addresses. The servo is controlled via PWM (GPIO 12), and each solenoid is activated by a GPIO pin through a transistor switch, powered by a 12V source with flyback diodes for protection. The Raspberry Pi acts as the central controller for sensing and actuation.

<img width="428" alt="image" src="https://github.com/user-attachments/assets/f38a16f3-f1e3-4bfd-9b36-1063663790eb" />


## Latency

### TOF Sensor

The VL53L4CD time-of-flight sensors are currently set to operate at a sampling frequency of 5 Hz. This rate is configured through the VL53L4CD_SetRangeTiming_RealTime() function by adjusting the timing_budget_ms and inter_measurement_ms parameters. A value of zero for inter_measurement_ms enables continuous ranging mode, while any non-zero value places the sensor into autonomous low-power mode.

Each distance measurement introduces an average latency of approximately 80–210 µs, which includes the interrupt handling, I2C communication, and the callback execution. At 5 Hz, this latency is well within the 200 ms window between samples. Increasing the sampling frequency could yield faster responsiveness, but doing so would also raise power consumption and increase bus traffic, particularly when multiple sensors are active on the same I2C line.

### Solenoid 

The solenoid valves are switched using GPIO pins and are controlled through a state-based logic system. To prevent both solenoids from opening simultaneously — which could overload the shared power supply — a 1 ms delay (usleep(1000)) was introduced between deactivating one valve and activating the other. This intentional delay ensures hardware safety and provides adequate buffer time for the electrical load to stabilize. The overall switching latency, including GPIO control and the enforced pause, is approximately 1.1–1.3 ms during transitions between solenoids.

### Servo Motor

The servo motor in the system is controlled using Pulse Width Modulation (PWM), where the pulse duration determines the motor’s position. PWM signals are generated using precise timing functions, and position changes are typically triggered by calling motor control functions. The effective latency from issuing a command to the motor reaching its new position is dependent on both signal propagation and mechanical response time, but the software-side delay is minimal — typically under 100–150 µs. However, due to the nature of PWM, smooth and accurate motion requires maintaining a stable frequency (usually 50 Hz), and excessively rapid updates may cause jitter or erratic behavior.


## Publication

An article about the project's purpose and functionality was published on **RS DesignSpark** on **20/03/2025**, highlighting its innovative approach to real-time liquid dispensing and sensor integration. The feature showcased the system's design, components, and potential applications in smart automation.
- [Article](https://www.rs-online.com/designspark/brugenie-coffee-anyway-you-wish)


## Social Media Channels

- [Instagram](https://www.instagram.com/brugenie/?locale=en_GB&hl=en)
- [Tiktok](https://www.tiktok.com/@brugenie)
- [Youtube](https://www.youtube.com/channel/UCvZ_YEZ7hOzSlOggeiQ_5xw)


### Platform Performance Summary:

- Instagram: 8.5K views, resulting in 85+ followers

- YouTube: 2K views, gaining 8+ followers

- TikTok: 300+ views, with 4 followers

- GitHub: 1,000 views and 53 repository clones

Total Views: Over 10,000 across platforms, garnering 100+ followers throughout the journey.

Key Insight: Instagram was the most successful platform in terms of engagement and reach, and therefore received the most strategic focus.
  
## Acknowledgements

We would like to thank Bernd Porr for providing us with the opportunity to work on this project and for supervising us throughout. We also extend our gratitude to RS Components for funding the project, their support played a key role in bringing our idea to life. Finally, a haertfelt thank you to the mechanical workshop, as well as the electronics and printing lab technicians, for their help in streamlining the build and completion of the system.

## Authors and Contributions

- **Luca Faccenda** - Completed the ADS1115 (weight sensor) class, Solenoid Controller, and VL53L4CD (time-of-flight) driver by translating it from C to C++. Also worked on volume control, motor control, and the pin control wrapper.
- **Manaf Habeeb** - Completed CAD drawings and models, 3D printing, unit testing, and social media outreach.
- **Tanvi Kapil** - Completed the ADS1115 (weight sensor) class, Solenoid Controller, and the VL53L4CD (time-of-flight) driver by translating it from C to C++. Also worked on volume control and motor control.
- **Taniya Jose** - Completed CAD drawings and models, 3D printing, Raspberry Pi setup, and wrote the RS DesignSpark article for the student fund. 
- **Maarven Pathmanabhan** - Completed all circuit design and wiring, and worked on the DS18B20 temperature sensor.

## License
### cppTimer
- The Timer Wrapper C++ was adopted from [Bernd Porr](https://github.com/berndporr), which can be sourced [here](https://github.com/berndporr/cppTimer).

### PWM
- The PWM C++ class was adopted from [Bernd Porr](https://github.com/berndporr), which can be sourced [here](https://github.com/berndporr/rpi_pwm#).

### ADS1115 
- The ADS1115 library was adopted from [Bernd Porr](https://github.com/berndporr), which can be sourced [here](https://github.com/berndporr/rpi_ads1115).

### VL53L4CD Time Of Flight
- The TOF sensor code was adopted from the driver provided by [ST Microelctronics](https://github.com/stmicroelectronics), which can be sourced [here](https://www.st.com/en/imaging-and-photonics-solutions/vl53l4cd.html#documentation).

## Future Work

The future work for this project would be to integrate the threading of both the dispensing and tofs to remove the need for user input when volume measurement is done. Addtionally a microswitch could be implemented to detect the presence of a mug. Additionally, the coffee could be heat and grided as part of the mehcanism. 



## Contact Us

Company Email: c0ffee.embedded@gmail.com

## Last Updated
This README was last updated on 21/04/2025.
