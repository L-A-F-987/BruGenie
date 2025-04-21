
#include "Parallax_INC4_Servo_Motor.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int Parallax_Motor::start(){
    pwm.start(channel, f); //starting the pwm and supplying frequency and GPIO channel
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
