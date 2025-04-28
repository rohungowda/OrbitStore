#include "../include/arena.h"
#include <cassert>

using namespace Orbit;

#include <iostream>
#include <iomanip>

/*
TODO

- allocate Allign
- Linked list
- freeSpace List

*/

Arena::Arena()
: ptr(nullptr), bytesRemain(0), totalSize(0)
{}

Arena::~Arena(){
    for(size_t i =0; i < blockHolder.size(); i++){
        delete[] blockHolder[i];
    }
}

// allign arranging data in memory so that data is stored at adress that is multiple of the size of data
// allows processors to be more efficient
char* Arena::AllocateAligned(size_t size){
    // haven't implemented AllocateAligned and FreeSpace linked list !!!
    assert(false);
    return nullptr; 
}

size_t Arena::getMemorySize(){
    return totalSize.load(std::memory_order_relaxed);
}

char* Arena::Allocate(size_t size){
    // check if there are enough bytes to allocate

    // if statment false throws a runtime error
    assert(size > 0 && size < BLOCKSIZE);

    std::unique_lock<std::mutex> lock(mtx);

    char* candidate = freespace.findCandidate(size);

    if(candidate != nullptr){
        return candidate;
    }
    
    if(bytesRemain <= size){
        ptr = AllocateBlock(BLOCKSIZE);
    }

    char* returnPtr = ptr;
    
    // add 1 to offset where the ptr will start from next
    ptr += size;
    bytesRemain -= size;

    totalSize.fetch_add(size, std::memory_order_relaxed);
    lock.unlock();


    return returnPtr;

}

char* Arena::AllocateBlock(size_t size)
{
    // add extra space to linkedlist only when the pointer is not null ( meaning that we are looking at remaning space)
    if(ptr != nullptr && bytesRemain > freespace.limit) {freespace.createEndNode(ptr, bytesRemain);}
    

    char* block = new char[size];
    bytesRemain = size;
    blockHolder.push_back(block);

    // operation is atomic with mem_order_relax but is not synchronized among threads
    //std::cout << std::hex << "block Adress: " << reinterpret_cast<uintptr_t>(block) << std::endl;
    return block;
}