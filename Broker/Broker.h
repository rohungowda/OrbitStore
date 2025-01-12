#ifndef BROKER_H
#define BROKER_H


#include <memory>
#include <iostream>


template <typename T>
class Broker  {
    public:
        Broker(){}

        void notifyService(){
            awake = true;
        }

        void ShareTask(T task){
            ProcessTask = task;
        }

        void ShareData(T data){
            ProcessedData = data;
        }

        void notifyWatcher(){
            awake = false;
        }

        void test(){
            std::cout << "Notified Broker" << std::endl;
        }

    private:
        bool awake = false;
        T ProcessTask;
        T ProcessedData;
};

#endif