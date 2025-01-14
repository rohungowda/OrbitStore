#ifndef SERVICE_H
#define SERVICE_H

template <typename T>
class Service
{
public:
    virtual boost::asio::awaitable<void> handle_task() = 0;

};


#endif