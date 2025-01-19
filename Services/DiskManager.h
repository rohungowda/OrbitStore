#ifndef DiskManager_H
#define DiskManager_H

#include "Service.h"
#include "../globals.h"

#include <iostream>

template <typename T>
class DiskManager : public Service<T>{

    public:
        DiskManager(){
            fd = open("data.bin", O_RDWR | O_CREAT | O_DIRECT, S_IRWXU);
        }

        ~DiskManager(){
            close(fd);
        }

        boost::asio::awaitable<void> handle_task(std::shared_ptr<T> data) override{

            if(data->getWriteStatus()){
                writePage(data->getPageID(), data->getBuffer()); 
            }else{
                readPage(data->getPageID(), data->getBuffer()); 
            }
            
            data->getPromise().set_value(true);
            co_return;
        }

        // functions to co_await
        void readPage(int pageID, char* buffer){
            off_t start = 0;
            ssize_t bytesRead = pread(fd, buffer, PAGE_SIZE, start);
            printTimestamp("Buffer read", bytesRead);
        }

        void writePage(int pageID, char* buffer){
            off_t start = 0;
            ssize_t bytesWritten = pwrite(fd, buffer, PAGE_SIZE, start);
             printTimestamp("Buffer Wrote", bytesWritten);
        }
        void delPage(int pageID, char* buffer);
        void allocatePage(int offset);

    private:
        std::unordered_map<int,off_t> pageTable;
        std::vector<int> freeslots;
        std::mutex io_latch;

        int fd;

};

#endif