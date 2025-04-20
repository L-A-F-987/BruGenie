#ifndef Thread_Controller_H_
#define Thread_Controller_H_

#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <atomic>



class Thread_Controller {

    friend class TOF_2callback;
    friend class Volume_Comms;

    explicit Thread_Controller(){};

    public:

        void lock_thread();

        void release();

    private:
    
        std::mutex mtx_;
        std::condition_variable cond_; 
        int i = 0;


};

#endif