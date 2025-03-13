#include "ds18b20.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sys/timerfd.h>

DS18B20::DS18B20(const std::string &deviceId, unsigned int intervalMs)
    : devicePath_("/sys/bus/w1/devices/" + deviceId + "/w1_slave")
    , intervalMs_(intervalMs)
    , timerFd_(-1)
    , running_(false)
    , currentTemperature_(0.0)
{
    // Create the timer file descriptor.
    timerFd_ = createTimerFd(intervalMs_);
    if (timerFd_ < 0) {
        throw std::runtime_error("Failed to create timerfd");
    }
}

DS18B20::~DS18B20()
{
    // Stop the worker thread and clean up.
    stop();
    if (timerFd_ >= 0) {
        close(timerFd_);
    }
}

void DS18B20::start()
{
    if (running_.load()) {
        return;
    }
    running_.store(true);
    thread_ = std::thread(&DS18B20::workerThread, this);
}

void DS18B20::stop()
{
    if (!running_.load()) {
        return;
    }
    running_.store(false);
    if (timerFd_ >= 0) {
        close(timerFd_);
        timerFd_ = -1;
    }
    if (thread_.joinable()) {
        thread_.join();
    }
}

double DS18B20::getTemperature() const
{
    return currentTemperature_;
}

int DS18B20::createTimerFd(unsigned int intervalMs)
{
    int fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (fd < 0) {
        std::cerr << "timerfd_create failed: " << strerror(errno) << std::endl;
        return -1;
    }

    struct itimerspec its;
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec = intervalMs / 1000;
    its.it_value.tv_nsec = (intervalMs % 1000) * 1000000ULL;
    its.it_interval.tv_sec = intervalMs / 1000;
    its.it_interval.tv_nsec = (intervalMs % 1000) * 1000000ULL;

    if (timerfd_settime(fd, 0, &its, nullptr) < 0) {
        std::cerr << "timerfd_settime failed: " << strerror(errno) << std::endl;
        close(fd);
        return -1;
    }
    return fd;
}

void DS18B20::workerThread()
{
    while (running_.load()) {
        uint64_t expirations = 0;
        ssize_t n = read(timerFd_, &expirations, sizeof(expirations));
        if (n == sizeof(expirations)) {
            readTemperature();
        } else {
            break;
        }
    }
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
}
