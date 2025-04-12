#include "../Parallax_INC4_Servo_Motor.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    
    Parallax_Motor motor;

    motor.start();

    motor.Set_motor_upwards();
    getchar();
    motor.Set_motor_stop();
    getchar();
    motor.Set_motor_downwards();
    getchar();
    motor.stop();
}