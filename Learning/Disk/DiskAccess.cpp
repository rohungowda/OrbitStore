#include <iostream>
#include <thread>
#include <condition_variable>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "../../globals.h"




class DiskAccess{
    public:
        DiskAccess()
        {
            database_thread = std::thread(&DiskAccess::handleRequests, this);
        }

        ~DiskAccess(){
            database_thread.join();
        }

        void handleRequests(){
            while(run){
                std::unique_lock<std::mutex> lock(mutex); // new lock on each iteration allows other threads to access
                cv.wait(lock,[this](){return disk_context.stopped(); });
                lock.unlock();

                disk_context.run();
            }

        }

        void acceptRequests(int pageID){
            char buffer[PageSize];
            std::cout << disk_context.stopped() << std::endl;
            boost::asio::post(disk_context, [this, pageID, &buffer]() {
                this->read(pageID, buffer);
            });
            std::cout << "Request Sent" << std::endl;
            std::cout << disk_context.stopped() << std::endl;

            std::unique_lock<std::mutex> lock(mutex);
            cv.notify_all();
            std::cout << "notification sent" << std::endl;
        }
    
    private:
        const static int PageSize = 4096;
        std::thread database_thread;
        std::mutex mutex;
        std::condition_variable cv;
        boost::asio::io_context disk_context;

        void read(int pageID, char* buffer){
            int byte_pointer = pageID * PageSize;
            std::cout << "Reading pageID " << byte_pointer << " into buffer address " << buffer << std::endl;
        }


        void write(int pageID, char* buffer){
            int byte_pointer = pageID * PageSize;
            std::cout << "Writing to pageID " << byte_pointer << " with data from buffer address " << buffer << std::endl; 
        }


};


int main(){
    DiskAccess manager = DiskAccess();
    manager.acceptRequests(5);
    return 0;
}