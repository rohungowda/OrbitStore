#include "IORequest.h"



IORequest::IORequest(bool wCheck, int page, char* buffer, std::promise<bool>& prom)
: pageID(page), bufferAdress(buffer), BufferPromise(prom), write(wCheck)
{}

int IORequest::getPageID(){
    return pageID;
}

char* IORequest::getBuffer(){
    return bufferAdress;
}

std::promise<bool>& IORequest::getPromise(){
    return BufferPromise;
}

bool IORequest::getWriteStatus(){
    return write;
}