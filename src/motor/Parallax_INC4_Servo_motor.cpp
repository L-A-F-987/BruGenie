#include "./rpi_pwm/rpi_pwm.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int channel = 2;
    int frequency = 40; // Hz
    if (argc > 1) {
	channel = atoi(argv[1]);
    }
    printf("Enabling PWM on channel %d.\n",channel);
    RPI_PWM pwm;
    pwm.start(channel, frequency);
    printf("Duty cycle at 65%%\n");
    pwm.setDutyCycle(5.2);
    getchar();
    printf("Duty cycle at 75%%\n");
    pwm.setDutyCycle(6);
    getchar();
    printf("Duty cycle at 85%%\n");
    pwm.setDutyCycle(6.8);
    getchar();
    pwm.stop();
}