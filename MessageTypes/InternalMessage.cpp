#include "InternalMessage.h"



InternalMessage::InternalMessage(int page, char* buffer, std::promise<char*>& prom)
: pageID(page), bufferAdress(buffer), BufferPromise(prom)
{}

int InternalMessage::getPageID(){
    return pageID;
}

char* InternalMessage::getBuffer(){
    return bufferAdress;
}

std::promise<char*>& InternalMessage::getPromise(){
    return BufferPromise;
}