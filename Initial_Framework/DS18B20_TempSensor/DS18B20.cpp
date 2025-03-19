
#include "DS18B20.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sys/timerfd.h>

void timerEvent();


DS18B20::DS18B20(const std::string &deviceId, unsigned int intervalMs)
    : devicePath_("/sys/bus/w1/devices/" + deviceId + "/w1_slave")
    , intervalMs_(intervalMs)
    , timerFd_(-1)
    , running_(false)
    , currentTemperature_(0.0)
{
    
}


void DS18B20::readTemperature()
{
    std::ifstream file(devicePath_);
    if (!file.is_open()) {
        std::cerr << "Failed to open DS18B20 device file: " << devicePath_ << std::endl;
        return;
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Error reading DS18B20 device file (line 1)" << std::endl;
        return;
    }
    if (line.find("YES") == std::string::npos) {
        std::cerr << "CRC check failed in DS18B20 read" << std::endl;
        return;
    }
    std::string tempLine;
    if (!std::getline(file, tempLine)) {
        std::cerr << "Error reading DS18B20 device file (line 2)" << std::endl;
        return;
    }
    auto pos = tempLine.find("t=");
    if (pos == std::string::npos) {
        std::cerr << "Temperature data not found" << std::endl;
        return;
    }
    std::string tempStr = tempLine.substr(pos + 2);
    try {
        int tempMilliC = std::stoi(tempStr);
        currentTemperature_ = static_cast<double>(tempMilliC) / 1000.0;
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse temperature: " << e.what() << std::endl;
    }

    std::cout << "Current DS18B20 temperature: " << currentTemperature_ << " °C" << std::endl;
}