#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>

#include "../Pin_Control_Wrapper.h"

int main() {

    printf("hello\n");

    Pin_Control solenoid_1;
    Pin_Control solenoid_2;

    solenoid_1.start(24);
    solenoid_2.start(25);

    solenoid_1.on();

    usleep(1000000);

    solenoid_1.off();
    solenoid_2.on();

    usleep(1000000);
    solenoid_2.off();
    usleep(1000000);

    printf("hello again\n");

    solenoid_1.stop();
    solenoid_2.stop();

}