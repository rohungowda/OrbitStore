#ifndef SERVICE_H
#define SERVICE_H

template <typename T>
class Service
{
public:
    virtual void handle_task() = 0;

};


#endif