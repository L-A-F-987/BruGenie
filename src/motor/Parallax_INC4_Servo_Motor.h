#ifndef Parallax_motor_H_
#define Parallax_motor_H_

#include "./rpi_pwm/rpi_pwm.h"


class Parallax_Motor {

    public:

    int start();

    int stop();

    int Set_motor_upwards();

    int Set_motor_stop();

    int Set_motor_downwards();

    private:

    //frequency for the PWM for each motor to get a gap of 20ms between pulses
    int f = 47;

    //channel for the PWM
    int channel = 2;

    //PWM
    RPI_PWM pwm;

};


#endif 