#include <iostream>
#include "ads1115rpi.h"

// Calibration constants (adjust these for your load cell)
const float CALIBRATION_FACTOR = 2666.67f; // [weight unit per Volt] (example value)
const float ZERO_OFFSET = 0.002f;         // Voltage offset when no load is applied

// Custom callback class to process ADS1115 samples
class LoadCellCallback : public ADS1115rpi::ADSCallbackInterface {
public:
    // This function is called each time a new voltage sample is available
    virtual void hasADS1115Sample(float sample) override {
        // Convert the voltage reading into a weight value
        float weight = (sample - ZERO_OFFSET) * CALIBRATION_FACTOR;
        std::cout << "Voltage: " << sample << " V, Weight: " << weight << " units" << std::endl;
    }
};

int main() {
    try {
        // Create an instance of the ADS1115 object
        ADS1115rpi adc;
        
        // Create our load cell callback object
        LoadCellCallback callback;
        adc.registerCallback(&callback);
        
        // Configure the ADS1115 settings
        ADS1115settings settings;
        settings.samplingRate = ADS1115settings::FS128HZ;  // For example: 128 Hz sampling rate
        settings.pgaGain      = ADS1115settings::FSR2_048;  // Full scale = 2.048 V (adjust if needed)
        settings.channel      = ADS1115settings::AIN0;      // Use analog input channel 0
        settings.drdy_chip    = 0;                          // GPIO chip number for RPi (1-4 use chip 0)
        settings.drdy_gpio    = DEFAULT_ALERT_RDY_TO_GPIO;    // Default ALERT/RDY pin (GPIO 17)
        
        // Start the ADC in the background. The worker thread will call our callback each time data is available.
        adc.start(settings);
        
        // Main thread runs an infinite loop (busy loop, no sleep calls) so the program remains active.
        while (true) {
            // Nothing to do here – all processing happens in the background callback.
            // In a real application you might add handling for termination signals.
        }
        
        // Optionally, you can stop the ADC:
        // adc.stop();
        
    } catch (const char* err) {
        std::cerr << "Error: " << err << std::endl;
        return 1;
    }
    
    return 0;
}
