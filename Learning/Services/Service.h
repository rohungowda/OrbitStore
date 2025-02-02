#ifndef SERVICE_H
#define SERVICE_H

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

template <typename T>
class Service
{
public:

    // function used as entry point for schedulers
    virtual boost::asio::awaitable<void> handle_task(std::shared_ptr<T> data) = 0;

};


#endif