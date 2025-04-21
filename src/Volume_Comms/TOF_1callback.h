

#ifndef TOF_1callback_H_
#define TOF_1callback_H_
#endif

#include "../VL53L4CD/user/uld-driver/VL53L4CD_api.h"

class TOF_1callback : public VL53L4CD_API::VL53L4CD_Callback_Interface {
   
    virtual void hasVL53L4CDSample(uint16_t v) override {
    printf("%i\n",v);

    if (variable) {
        variable -> store(v);
    }

    };

    public:

        std::atomic<int>* variable;

};
