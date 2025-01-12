#ifndef SCHEDULER_H
#define SCHEDULER_H 

#include "../../globals.h"

#include "Comparators.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <functional>

template<typename T, typename Compare>
class Scheduler{
    public:
        Scheduler(LogicalPointer<T> &logic, Compare comp,  std::function<void(T)> async_response)
        : pq(comp), pointer(logic), stored(async_response)
            {
                consumer_thread = std::thread(&Scheduler::ConsumeMessages, this);
            }


        ~Scheduler(){
            consumer_thread.join();
        }

        void Push(T data){
            std::unique_lock<std::mutex> lock(mtx);
            pq.push(data);
            lock.unlock();

            cv.notify_all();
        }

        void ConsumeMessages(){
            while(run){
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock,[this]() { return !pq.empty();});
                

                T task = pq.top();
                pq.pop();
                pointer.setLogicalPointer(task);
                lock.unlock();
                stored(task);

            }
        }
    
    private:
        std::priority_queue<T, std::vector<T>, Compare> pq;
        std::thread consumer_thread;
        std::mutex mtx;
        std::condition_variable cv;
        LogicalPointer<T> &pointer;
        std::function<void(T)> stored;
};

#endif 