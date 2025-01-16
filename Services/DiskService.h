#ifndef DISKSERVICE_H
#define DISKSERVICE_H

#include "Service.h"

#include <iostream>

template <typename T>
class DiskService : public Service<T>{

    public:
        DiskService(){}

        boost::asio::awaitable<void> handle_task(std::shared_ptr<T>& data) override{
            data->getPromise().set_value(data->getBuffer());
            std::cout << "CallBack function set" << std::endl;
            co_return;
        }
};

#endif