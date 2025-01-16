#ifndef SERVICE_H
#define SERVICE_H

template <typename T>
class Service
{
public:
    // basically this needs to take in a promise and any messageData, once it processes it will set the promise value
    // either make a callback virtual function or let each service define it, most likely callback
    virtual boost::asio::awaitable<void> handle_task(std::shared_ptr<T>& data) = 0;

};


#endif