#ifndef DS18B20_H
#define DS18B20_H

#include <string>
#include <thread>
#include <atomic>

class DS18B20
{
public:
    /**
     * @brief Constructor
     * @param deviceId The 1-Wire device ID (e.g., "28-01193a2bd1aa")
     * @param intervalMs The read interval in milliseconds
     */
    DS18B20(const std::string &deviceId, unsigned int intervalMs = 1000);

    /**
     * @brief Destructor - stops the thread and cleans up resources.
     */
    ~DS18B20();

    /**
     * @brief Start reading temperatures at the specified interval.
     */
    void start();

    /**
     * @brief Stop reading temperatures.
     */
    void stop();

    /**
     * @brief Get the most recently read temperature in Celsius.
     * @return Temperature in Celsius.
     */
    double getTemperature() const;

private:
    /**
     * @brief Main worker thread function that blocks on the timerfd.
     */
    void workerThread();

    /**
     * @brief Create a timerfd that periodically expires at the given interval.
     * @param intervalMs The interval in milliseconds.
     * @return A valid file descriptor on success, or -1 on failure.
     */
    int createTimerFd(unsigned int intervalMs);

    /**
     * @brief Perform a single temperature read from the DS18B20 sensor.
     */
    void readTemperature();

private:
    std::string devicePath_;        ///< Path to the DS18B20 device in /sys
    unsigned int intervalMs_;       ///< Interval in milliseconds for reads
    int timerFd_;                   ///< Timer file descriptor
    std::thread thread_;            ///< Worker thread
    std::atomic<bool> running_;     ///< Flag indicating if the worker is running
    double currentTemperature_;     ///< Last read temperature in Celsius
};

#endif // DS18B20_H
