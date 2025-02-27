#ifndef IORequest_H
#define IORequest_H

#include <future>

class IORequest{
    public:
        // promise returns a buffer
        IORequest(bool write, int page, char* buffer, std::promise<bool>& prom);

        bool getWriteStatus();

        int getPageID();

        char* getBuffer();

        std::promise<bool>& getPromise();

    
    private:
        bool write;
        int pageID;
        char* bufferAdress; // pointer to the buffer
        std::promise<bool>& BufferPromise;
};

#endif