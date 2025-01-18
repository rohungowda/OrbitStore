#ifndef IORequest_H
#define IORequest_H

#include <future>

class IORequest{
    public:
        // promise returns a buffer
        IORequest(int page, char* buffer, std::promise<char*>& prom);

        int getPageID();

        char* getBuffer();

        std::promise<char*>& getPromise();

    
    private:
        int pageID;
        char* bufferAdress; // pointer to the buffer
        std::promise<char*>& BufferPromise;
};

#endif