/*
written for weight sensor with the 
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "./rpi_ads1115/ads1115rpi.h"

// We inherit ADS1115rpi, implement
// hasSample() and print the ADC reading.
class ADS1115Printer : public ADS1115rpi::ADSCallbackInterface {
	virtual void hasADS1115Sample(float v) override {
		printf("%e\n",v);
	}
};

// Creates an instance of the ADS1115Printer class.
// Prints data till the user presses a key.
int main(int argc, char *argv[]) {
	fprintf(stderr,"Press any key to stop.\n");
	ADS1115Printer ads1115Callback;
	ADS1115rpi ads1115rpi;
	ads1115rpi.registerCallback(&ads1115Callback);
        ADS1115settings s;
	s.samplingRate = ADS1115settings::FS8HZ;
    //setting the input gain 
	s.drdy_chip = 4; // for RPI1-4 chip = 0. For RPI5 it's chip = 4.
	ads1115rpi.start(s);
        fprintf(stderr,"fs = %d\n",ads1115rpi.getADS1115settings().getSamplingRate());
	getchar();
	ads1115rpi.stop();
	return 0;
}
