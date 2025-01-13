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
        sharedbroker->setupService(gate);
        service_thread = std::thread(&ServiceInterface::startThread, this, std::ref(serviceType), std::ref(sharedbroker));
    }

    void startThread(const std::shared_ptr<Service<T>> & serviceType, const std::shared_ptr<Broker<T>> &sharedbroker){
        while(run){
                

                std::unique_lock<std::mutex> lock(gate_mtx);
                gate.wait(lock,[this, sharedbroker]() { return (sharedbroker->getAwake());});
                
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::cout << sharedbroker->getTask() << " Task handled" << std::endl;

                sharedbroker->notifyWatcher(sharedbroker->getTask());

            }
        
    }


private:
    std::thread consumer_thread;
    std::mutex gate_mtx;
    std::condition_variable gate;
    std::thread service_thread;
};



#endif