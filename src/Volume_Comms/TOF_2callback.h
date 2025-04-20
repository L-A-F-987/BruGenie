
#ifndef TOF_2callback_H_
#define TOF_2callback_H_
#endif

#include "../VL53L4CD/user/uld-driver/VL53L4CD_api.h"
#include <mutex>

class TOF_2callback : public VL53L4CD_API::VL53L4CD_Callback_Interface {
    virtual void hasVL53L4CDSample(uint16_t v std::mutex mut) override {
    
    mut.lock(;)
    printf("%i\n",v);

    if (variable) {
        variable -> store(v);
    }

    };

    mut.unlock();

    public:

        std::atomic<int>* variable;
        std::mutex mut;

};

