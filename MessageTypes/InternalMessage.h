#ifndef INTERNALMESSAGE_H
#define INTERNALMESSAGE_H

#include <future>

class InternalMessage{
    public:
        // promise returns a buffer
        InternalMessage(int page, char* buffer, std::promise<char*>& prom);

        int getPageID();

        char* getBuffer();

        std::promise<char*>& getPromise();

    
    private:
        int pageID;
        char* bufferAdress; // pointer to the buffer
        std::promise<char*>& BufferPromise;
};

#endif