#ifndef MESSAGEQUEUEINTERFACE_H
#define MESSAGEQUEUEINTERFACE_H

#include "../Broker/Broker.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>


template <typename T>
class MessageQueueInterface
{
public:

    MessageQueueInterface(const std::shared_ptr<Broker<T>> &sharedbroker)
    {
        startThread(sharedbroker); // only pointer reference is const, can't change where it is pointing to
    }

    void startThread(const std::shared_ptr<Broker<T>> &sharedbroker){
        std::cout << "MessageQueue Interface" << std::endl;
        sharedbroker->test();
        
    }


private:
    std::thread consumer_thread;
    std::mutex mtx;
    std::condition_variable cv;
};



#endif