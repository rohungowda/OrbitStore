#ifndef DISKSERVICE_H
#define DISKSERVICE_H

#include "Service.h"

#include <iostream>

template <typename T>
class DiskService : public Service<T>{

    public:
        DiskService(){}

        boost::asio::awaitable<void> handle_task() override{
            std::cout << "Disk Service Stackless Coroutine" << std::endl;
            co_return;
        }
};

#endif