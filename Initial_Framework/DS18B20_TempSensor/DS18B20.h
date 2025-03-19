
#ifndef DS18B20_H
#define DS18B20_H

#include <string>
#include <thread>
#include <atomic>
#include "./cppTimer/CppTimer.h"

class DS18B20 : public CppTimer {

	void timerEvent() {
        readTemperature();
    }

public:
DS18B20(const std::string &deviceId, unsigned int intervalMs = 1000);

private:
    std::string devicePath_;        ///< Path to the DS18B20 device in /sys
    unsigned int intervalMs_;       ///< Interval in milliseconds for reads
    int timerFd_;                   ///< Timer file descriptor
    std::thread thread_;            ///< Worker thread
    std::atomic<bool> running_;     ///< Flag indicating if the worker is running
    double currentTemperature_;     ///< Last read temperature in Celsius


    void readTemperature();
};


#endif // DS18B20_H
