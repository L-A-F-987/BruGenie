#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>

#include "../Pin_Control_Wrapper.h"

int main() {


    /*
    printf("hello\n");

    Pin_Control solenoid_1;
    Pin_Control solenoid_2;

    solenoid_1.start(24);
    solenoid_2.start(25);


    for(int i = 0;i<30;i++){
    solenoid_1.on();

    usleep(100000);

    solenoid_1.off();
    usleep(100000);
    solenoid_2.on();

    usleep(100000);
    solenoid_2.off();
    usleep(100000);

    }

    solenoid_1.stop();
    solenoid_2.stop();

    */

    Pin_Control x_shut_6;
    x_shut_6.start(5);
    usleep(1000);
    x_shut_6.on();
    usleep(1000);
    x_shut_6.stop();
}