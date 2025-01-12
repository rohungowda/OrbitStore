#ifndef DISKSERVICE_H
#define DISKSERVICE_H

#include "Service.h"

#include <iostream>

template <typename T>
class DiskService : public Service<T>{

    public:
        DiskService(){}

        void handle_task() override{
            std::cout << "Disk Service" << std::endl;
        }
};

#endif