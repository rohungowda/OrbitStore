#ifndef SERVICECLASS_H
#define SERVICECLASS_H

#include "Service.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/bind/bind.hpp>


template <typename T>
class ServiceScheduler
{
public:

    ServiceScheduler(std::shared_ptr<Service<T>> serviceType)
    :  ServiceManager(serviceType)
    {
        // the lambda will reutn a std::function<awaitable type>
        service_thread = std::thread(&ServiceScheduler::startThread, this);
    }

    void notify(std::shared_ptr<T> data){
        boost::asio::co_spawn(io_context, ServiceManager->handle_task(data), boost::asio::detached);
        //printTimestamp("Recieved Task  ", data->getPageID());
        //io_context.post([this, data](){ printWorld(data); });
    }

    void printWorld(std::shared_ptr<T> data){
        printTimestamp(" Hello World from ", data->getPageID());
    }

    void startThread(){
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(io_context.get_executor());
        io_context.run();
    }

    ~ServiceScheduler(){
        io_context.stop();
        service_thread.join();
    }

private:
    std::thread service_thread;
    boost::asio::io_context io_context;
    const std::shared_ptr<Service<T>> ServiceManager;
};



#endif