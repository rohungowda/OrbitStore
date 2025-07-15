#include "../include/memtable.h"

#include <iostream>

using namespace Orbit;

MemTable::MemTable() {}

MemTable::~MemTable() {}


UserRequest MemTable::GET(UserRequest req){

    // create InternalKeyComparator
    for(char * ptr : TempDataStructure){
        Slice key = getInternalKey(ptr);
        if(key == req.getKey()){
            return decode(ptr);
        }
    }

    return UserRequest();

}

void MemTable::PUT(UserRequest req){

    size_t messageSize = req.getLength();
    char *ptr = allocater.Allocate(messageSize);
    encode(req,ptr);

    // locks on creation
    std::lock_guard<std::mutex> lock(MemTable_mtx);
    TempDataStructure.push_back(ptr);
}
