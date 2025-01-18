#include "globals.h"
#include "Services/ServiceScheduler.h"
#include "Services/DiskManager.h"
#include "MessageTypes/IORequest.h"

#include <iostream>
#include <thread>

int main(){

    // Create a Factory Class which will Instantiate everything
    std::shared_ptr<ServiceScheduler<IORequest>> diskInterface =  std::make_shared<ServiceScheduler<IORequest>>(std::make_shared<DiskManager<IORequest>>());
    // in actual memory buffer we want to dynamically allocate it
    char buffer1[PAGE_SIZE];
    char buffer2[PAGE_SIZE];
    char buffer3[PAGE_SIZE];
    char buffer4[PAGE_SIZE]; 


    std::thread t1([&diskInterface,&buffer1, &buffer3,  &buffer2,  &buffer4](){
         std::promise<char*> p;
        std::future<char*> f = p.get_future();

    diskInterface->notify(std::make_shared<IORequest>(5, buffer1, std::ref(p)));
    diskInterface->notify(std::make_shared<IORequest>(20, buffer2, std::ref(p)));
    diskInterface->notify(std::make_shared<IORequest>(15, buffer3, std::ref(p)));
    diskInterface->notify(std::make_shared<IORequest>(25, buffer4, std::ref(p)));
    std::this_thread::sleep_for(std::chrono::seconds(5));

    });
        t1.join();



    printTimestamp("program ends with status ", 0);
    return 0;
}

/*
 diskQueue.Push(std::make_shared<IORequest>(14,buffer3,std::ref(p)));
        diskQueue.Push(std::make_shared<IORequest>(120,buffer1,std::ref(p)));
        diskQueue.Push(std::make_shared<IORequest>(14,buffer3,std::ref(p)));
        diskQueue.Push(std::make_shared<IORequest>(120,buffer1,std::ref(p)));
        std::this_thread::sleep_for(std::chrono::seconds(2));
        diskQueue.Push(std::make_shared<IORequest>(14,buffer3,std::ref(p)));
        diskQueue.Push(std::make_shared<IORequest>(120,buffer1,std::ref(p)));
        
        std::cout << f.get() << std::endl;
*/