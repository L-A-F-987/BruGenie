
#include "Parallax_INC4_Servo_Motor.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int Parallax_Motor::start(){
    pwm.start(channel, f); //starting the pwm and supplying frequency and GPIO channel

    return 0;
}


int Parallax_Motor::Set_motor_upwards(){
     pwm.setDutyCycle(6.1);

     return 0;
};

int Parallax_Motor::Set_motor_stop(){
    pwm.setDutyCycle(7.05);

    return 0;
    
};

int Parallax_Motor::Set_motor_downwards(){
    pwm.setDutyCycle(8);

    return 0;
    
};


int Parallax_Motor::stop(){
    pwm.stop();

    return 0;
};
