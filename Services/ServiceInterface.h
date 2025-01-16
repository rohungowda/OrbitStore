#ifndef SERVICECLASS_H
#define SERVICECLASS_H

#include "../Broker/Broker.h"
#include "Service.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>


template <typename T>
class ServiceInterface
{
public:

    ServiceInterface(const std::shared_ptr<Service<T>> & serviceType, const std::shared_ptr<Broker<T>> &sharedbroker)
    {
        // the lambda will reutn a std::function<awaitable type>
        sharedbroker->setupService(gate, io_context, [serviceType](std::shared_ptr<T>& data) { return serviceType->handle_task(data); });
        service_thread = std::thread(&ServiceInterface::startThread, this, std::ref(serviceType), std::ref(sharedbroker));
    }

    void startThread(const std::shared_ptr<Service<T>> & serviceType, const std::shared_ptr<Broker<T>> &sharedbroker){
        while(run){
                

                std::unique_lock<std::mutex> lock(gate_mtx);
                gate.wait(lock,[this, sharedbroker]() { return (sharedbroker->getAwake());});
                
                io_context.run();

                std::cout << " All Task handled" << std::endl;

                sharedbroker->notifyWatcher();

            }
        
    }


private:
    std::thread consumer_thread;
    std::mutex gate_mtx;
    std::condition_variable gate;
    std::thread service_thread;
    boost::asio::io_context io_context;
};



#endif