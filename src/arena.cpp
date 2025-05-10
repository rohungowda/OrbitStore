#include "../include/arena.h"
#include <cassert>

using namespace Orbit;

#include <iostream>
#include <iomanip>


Arena::Arena()
: ptr(nullptr), bytesRemain(0), totalSize(0)
{}

Arena::~Arena(){
    for(size_t i =0; i < blockHolder.size(); i++){
        delete[] blockHolder[i];
    }
}

// TODO
char* Arena::AllocateAligned(size_t size){
    assert(false);
    return nullptr; 
}

size_t Arena::getMemorySize(){
    return totalSize.load(std::memory_order_relaxed);
}

char* Arena::Allocate(size_t size){

    // if statment false throws a runtime error - ensures data is within range of limit
    assert(size > 0 && size <= BLOCKSIZE);

    // Possible Lock-free option in future? Problem is the delete when a node size becomes 0
    // Harris 2001. A Pragmatic Implementation of Non-Blocking Linked-Lists 
    std::unique_lock<std::mutex> lock(mtx);

    char* candidate = freespace.findCandidate(size);

    if(candidate != nullptr){ return candidate; }

    if(bytesRemain < size){ ptr = AllocateBlock(BLOCKSIZE);}

    char* returnPtr = ptr;
    ptr += size;
    bytesRemain -= size;

    totalSize.fetch_add(size, std::memory_order_relaxed);


    return returnPtr;

}

char* Arena::AllocateBlock(size_t size)
{
    // add extra space to linkedlist only when the pointer is not null ( meaning that we are looking at remaning space)
    if(ptr != nullptr && bytesRemain > freespace.limit) {freespace.createEndNode(ptr, bytesRemain);}
    

    char* block = new char[size];
    bytesRemain = size;
    blockHolder.push_back(block);

    // operation is atomic with mem_order_relax but is not synchronized among threads - it will eventually get to teh right value
    return block;
}