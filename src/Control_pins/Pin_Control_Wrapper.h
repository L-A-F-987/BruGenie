


#ifndef Solenoid_Control_H_
#define Solenoid_Control_H_

#define chip_used 4


#include <stdio.h>

//adding cpp class 
class Pin_Control{



    public:

    int on();

    int off();

    int start(int pin_number);

    int stop();




    private:

    struct gpiod_line *pinGPIO = nullptr;
	struct gpiod_chip *chipGPIO = nullptr;
};

#endif


