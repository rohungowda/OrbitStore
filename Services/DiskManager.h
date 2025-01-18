#ifndef DiskManager_H
#define DiskManager_H

#include "Service.h"

#include <iostream>
#include <fstream>

template <typename T>
class DiskManager : public Service<T>{

    public:
        DiskManager(){}

        boost::asio::awaitable<void> handle_task(std::shared_ptr<T> data) override{
            //data->getPromise().set_value(data->getBuffer());
            printTimestamp("Disk Service", data->getPageID());
            co_return;
        }

        // functions to co_await
        void readPage(int pageID, char* buffer);
        void writePage(int pageID, char* buffer);
        void delPage(int pageID, char* buffer);
        void allocatePage(int offset);

    private:
        std::unordered_map<int,int> pageTable;
        std::vector<int> freeslots;
        std::mutex io_latch;

        std::fstream db_file; // each buffer pool has an access to a Disk Scheduler which runs on the thread that calls it

};

#endif