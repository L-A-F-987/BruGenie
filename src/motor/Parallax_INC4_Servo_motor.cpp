
#include "Parallax_INC4_Servo_Motor.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int Parallax_Motor::start(){
    pwm.start(channel, f);
}


int Parallax_Motor::Set_motor_upwards(){
     pwm.setDutyCycle(6.1);
};

int Parallax_Motor::Set_motor_stop(){
    pwm.setDutyCycle(7.05);
    
};

int Parallax_Motor::Set_motor_downwards(){
    pwm.setDutyCycle(8);
    
};


int Parallax_Motor::stop(){
    pwm.stop();
};


/*
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
*/
