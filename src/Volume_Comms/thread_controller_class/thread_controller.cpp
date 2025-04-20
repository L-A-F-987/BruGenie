
#include "thread_controller.h"
//function to lock thread/threads
void Thread_Controller::lock_thread(){
    std::unique_lock<std::mutex> locker(mtx_);
        cond_.wait(locker, [this]() {
            return i == 1;
        });
};

//function to release a thread
void Thread_Controller::release(){
    cond_.notify_one();
    
};