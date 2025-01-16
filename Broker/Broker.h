#ifndef BROKER_H
#define BROKER_H


#include <memory>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

template <typename T>
class Broker  {
    public:
        Broker()
        : messageLength(0)
        {}

        void notifyService(std::shared_ptr<T> task, size_t currentLength){
            std::unique_lock<std::mutex> lock(mutex);
            awake = true;
            messageLength = currentLength;

            boost::asio::co_spawn(*contextReference,[this, &task](){
                return saved_handler(task);
            } , boost::asio::detached);

            ServiceCondition->notify_one();

        }

        void notifyWatcher(){
            std::unique_lock<std::mutex> lock(mutex);
            if(messageLength == 0){
                awake = false;
            }
        }

        bool getAwake(){
            return awake;
        }

        void setupService(std::condition_variable &cv, boost::asio::io_context &context, std::function<boost::asio::awaitable<void>(std::shared_ptr<T>& data)> async_handler){
            ServiceCondition = &cv;
            contextReference = &context;
            saved_handler = async_handler;
        }


    private:
        bool awake = false;
        size_t messageLength;
        std::condition_variable* ServiceCondition = nullptr;
        std::mutex mutex;

        boost::asio::io_context* contextReference;
        std::function<boost::asio::awaitable<void>(std::shared_ptr<T>& data)> saved_handler; // right now no paramters need to change that depending on the type
        // completion handler too

};

#endif

/*
        bool getConsume(){
            return consume;
        }
        bool consume = false;

                T ProcessTask;
        T ProcessedData;

        void setupWatcher(std::condition_variable &cv){
            WatcherCondition = &cv;
        }

                //std::condition_variable* WatcherCondition = nullptr;

*/