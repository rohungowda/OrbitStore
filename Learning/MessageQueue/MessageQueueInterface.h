#ifndef MESSAGEQUEUEINTERFACE_H
#define MESSAGEQUEUEINTERFACE_H

#include "../Broker/Broker.h"
#include "../globals.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <queue>


template <typename T>
class MessageQueueInterface
{
public:

    MessageQueueInterface(const std::shared_ptr<Broker<T>> &sharedbroker)
    {
        consumer_thread = std::thread(&MessageQueueInterface::startThread, this, std::ref(sharedbroker));
    }


    ~MessageQueueInterface(){
        consumer_thread.join();
    }


    void Push(std::shared_ptr<T> task){
        std::unique_lock<std::mutex> lock(mtx);
        messageQueue.push(task);
        lock.unlock();
        cv.notify_one();
    }

    void startThread(const std::shared_ptr<Broker<T>> &sharedbroker){
        while(run){
                

                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock,[this]() { return !messageQueue.empty();});
                std::shared_ptr<T> task = messageQueue.front();
                messageQueue.pop();
                size_t length= messageQueue.size();
                lock.unlock();
                sharedbroker->notifyService(task);

            }
        
    }

    void Pass(){

    }


private:
    std::thread consumer_thread;
    std::mutex mtx;
    std::mutex gate_mtx;
    std::condition_variable cv;
    std::condition_variable gate;
    std::thread messageThread;
    std::queue<std::shared_ptr<T>> messageQueue;
    //std::shared_ptr<Broker<T>> &messageBroker;
};



#endif