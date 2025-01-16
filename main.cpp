#include "globals.h"
#include "Broker/Broker.h"
#include "Services/ServiceInterface.h"
#include "Services/DiskService.h"
#include "MessageQueue/MessageQueueInterface.h"
#include "MessageTypes/InternalMessage.h"

#include <iostream>
#include <thread>

int main(){

    // Create a Factory Class which will Instantiate everything
    std::shared_ptr<Broker<InternalMessage>> DiskBroker = std::make_shared<Broker<InternalMessage>>();
    ServiceInterface<InternalMessage> disk = ServiceInterface<InternalMessage>(std::make_shared<DiskService<InternalMessage>>(), DiskBroker);
    MessageQueueInterface<InternalMessage> diskQueue = MessageQueueInterface<InternalMessage>(DiskBroker);

    std::cout << "Succesfully Created" << std::endl << std::endl;

    // in actual memory buffer we want to dynamically allocate it
    char buffer1[PAGE_SIZE];
    char buffer2[PAGE_SIZE];
    char buffer3[PAGE_SIZE];

    std::thread t1([&diskQueue, &DiskBroker, &buffer1](){
         std::promise<char*> p;
        std::future<char*> f = p.get_future();

        std::this_thread::sleep_for(std::chrono::seconds(4));
        diskQueue.Push(std::make_shared<InternalMessage>(5,buffer1,std::ref(p)));


    });

    t1.join();

    return 0;
}