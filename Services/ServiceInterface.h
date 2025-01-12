#ifndef SERVICECLASS_H
#define SERVICECLASS_H

#include "../Broker/Broker.h"
#include "Service.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>


template <typename T>
class ServiceInterface
{
public:

    ServiceInterface(const std::shared_ptr<Service<T>> & serviceType, const std::shared_ptr<Broker<T>> &sharedbroker)
    {
        startThread(serviceType, sharedbroker); // only pointer reference is const, can't change where it is pointing to
    }

    void startThread(const std::shared_ptr<Service<T>> & serviceType, const std::shared_ptr<Broker<T>> &sharedbroker){
        std::cout << "Server Interface" << std::endl;
        serviceType->handle_task();
        sharedbroker->test();
        
    }


private:
    std::thread consumer_thread;
    std::mutex mtx;
    std::condition_variable cv;
};



#endif