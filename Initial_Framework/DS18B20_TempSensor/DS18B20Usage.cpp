#include <iostream>
#include "ds18b20.h"
#include <unistd.h> // for sleep()

int main()
{
    try {
        // Replace with actual DS18B20 device ID from /sys/bus/w1/devices/
        DS18B20 sensor("28-01193a2bd1aa", 2000); // Read every 2 seconds
        sensor.start();

        // Run for approximately 10 seconds
        for (int i = 0; i < 5; ++i) {
            sleep(2);
            double tempC = sensor.getTemperature();
            std::cout << "Current DS18B20 temperature: " << tempC << " °C" << std::endl;
        }
        sensor.stop();
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
