#include "../include/arena.h"
#include <cassert>

using namespace Orbit;

/*maybe keep space in sorted order and do binary search - some reseasrch adn planning needed*/
/*Implement freespace list - maybe not a vector? and test regular allocation and byte use with mulyiple threads. Then do Aligned allocation and use*/

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

}

size_t Arena::getMemorySize(){
    return totalSize.load(std::memory_order_relaxed);
}

char* Arena::Allocate(size_t size){
    // check if there are enough bytes to allocate

    assert(size > 0);

    std::unique_lock<std::mutex> lock(mtx);

    if(bytesRemain < size){
        ptr = AllocateBlock(BLOCKSIZE);
    }

    char* returnPtr = ptr;
    ptr += size;
    bytesRemain -= size;
    lock.unlock();

    return returnPtr;

}

char* Arena::AllocateBlock(size_t size)
{
    char* block = new char[size];
    bytesRemain = size;
    blockHolder.push_back(block);

    // operation is atomic with mem_order_relax but is not synchronized among threads
    totalSize.fetch_add(size + sizeof(char*), std::memory_order_relaxed);

    return block;
}