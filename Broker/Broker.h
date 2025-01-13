#ifndef BROKER_H
#define BROKER_H


#include <memory>
#include <iostream>
#include <mutex>
#include <condition_variable>


template <typename T>
class Broker  {
    public:
        Broker(){}

        void notifyService(T task){
            ProcessTask = task;
            awake = true;
            consume = false;
            ServiceCondition->notify_one();
        }

        void notifyWatcher(T data){
            ProcessedData = data;
            awake = false;
            consume = true;
            WatcherCondition->notify_one();
        }

        bool getAwake(){
            return awake;
        }

        bool getConsume(){
            return consume;
        }

        void setupWatcher(std::condition_variable &cv){
            WatcherCondition = &cv;
        }

        void setupService(std::condition_variable &cv){
            ServiceCondition = &cv;
        }

        T getTask(){
            return ProcessTask;
        }

    private:
        bool awake = false;
        bool consume = false;
        T ProcessTask;
        T ProcessedData;
        std::condition_variable* WatcherCondition = nullptr;
        std::condition_variable* ServiceCondition = nullptr;
};

#endif