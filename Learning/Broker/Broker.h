#ifndef BROKER_H
#define BROKER_H


#include <memory>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "../Services/ServiceInterface.h"

template <typename T>
class Broker  {
    public:
        Broker(std::shared_ptr<ServiceInterface<T>> Serv)
        :ServiceManager(Serv)
        {}

        void notifyService(std::shared_ptr<T> task){
            printTimestamp("Broker Notifies Service", task->getPageID());
            ServiceManager->notify(task);

        }


    private:
        std::shared_ptr<ServiceInterface<T>> ServiceManager;


};

#endif
