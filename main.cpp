#include "globals.h"
#include "Broker/Broker.h"
#include "Services/ServiceInterface.h"
#include "Services/DiskService.h"
#include "MessageQueue/MessageQueueInterface.h"

#include <iostream>
#include <thread>

int main(){

    // can create this with factory class
    std::shared_ptr<Broker<int>> DiskBroker = std::make_shared<Broker<int>>();
    ServiceInterface<int> disk = ServiceInterface<int>(std::make_shared<DiskService<int>>(), DiskBroker);
    MessageQueueInterface<int> diskQueue = MessageQueueInterface<int>(DiskBroker);

    std::cout << "Succesfully Created" << std::endl << std::endl;

    std::thread t1([&diskQueue, &DiskBroker](){
        diskQueue.Push(1);

    });

    std::thread t2([&diskQueue, &DiskBroker](){
        diskQueue.Push(2);
        diskQueue.Push(3);

    });

    t1.join();
    t2.join();

    return 0;
}