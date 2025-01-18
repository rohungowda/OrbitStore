#include "IORequest.h"



IORequest::IORequest(int page, char* buffer, std::promise<char*>& prom)
: pageID(page), bufferAdress(buffer), BufferPromise(prom)
{}

int IORequest::getPageID(){
    return pageID;
}

char* IORequest::getBuffer(){
    return bufferAdress;
}

std::promise<char*>& IORequest::getPromise(){
    return BufferPromise;
}