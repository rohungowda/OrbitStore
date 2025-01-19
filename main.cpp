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
    alignas(PAGE_SIZE) char buffer1[PAGE_SIZE];
    alignas(PAGE_SIZE) char buffer2[PAGE_SIZE];

    for(int i = 0; i < PAGE_SIZE; i++){
        buffer1[i] = static_cast<char>(5);
    }


    std::promise<bool> p;
    std::future<bool> f = p.get_future();

    diskInterface->notify(std::make_shared<IORequest>(true, 5, buffer1, std::ref(p)));
    f.get();

    std::promise<bool> pa;
    std::future<bool> fa = pa.get_future();

    diskInterface->notify(std::make_shared<IORequest>(false, 5, buffer2, std::ref(pa)));
    fa.get();



    for(int i = 0; i < 20; i++){
        std::cout <<"Hello " <<  buffer2[i] << "";
    }
    std::cout << std::endl;


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