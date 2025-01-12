#include "globals.h"
#include "Broker/Broker.h"
#include "Services/ServiceInterface.h"
#include "Services/DiskService.h"
#include "MessageQueue/MessageQueueInterface.h"

#include <iostream>

int main(){

    // can create this with factory class
    std::shared_ptr<Broker<int>> DiskBroker = std::make_shared<Broker<int>>();
    ServiceInterface<int> disk = ServiceInterface<int>(std::make_shared<DiskService<int>>(), DiskBroker);
    MessageQueueInterface<int> diskQueue = MessageQueueInterface<int>(DiskBroker);

    std::cout << "Succesfully Created" << std::endl;

    return 0;
}