
#include <iostream>
#include "DS18B20.h"
#include <unistd.h> // for sleep()

int main()
{
   
    // Replace with actual DS18B20 device ID from /sys/bus/w1/devices/
    DS18B20 sensor("28-00000ff865f9"); // Read every 2 seconds
	// every 500000ns
    sensor.startms(200);
    sensor.stop();
	
}
