#ifndef ARENA_H
#define ARENA_H

#include <cstddef>
#include <vector>
#include <atomic>
#include <mutex>
#include "LinkedList.h"

#include <iostream>
using namespace std;

// Arena Class will have a quote on quote parent that controls access, so no need for mutexes in arena
namespace Orbit{
    class Arena{
        public:
            Arena();

            ~Arena();

            // Allocates some amount of memory needed and return a pointer to the mnemory
            char * Allocate(size_t size);

            // get memory size
            size_t getMemorySize();


        private:

            // Local Variables for Block
            char* ptr;
            size_t bytesRemain;

            // Block holder array ( Could use Linked List ) - better to use linked list
            std::vector<char*> blockHolder;

            // totalSize
            std::atomic<size_t> totalSize;
            
            const size_t BLOCKSIZE = 1024;
            
            
            // allocates a large memory block of BlockSize  and returns a pointer
            char * AllocateBlock(size_t size);

            // TODO Implement
            char* AllocateAligned(size_t size);

            

    };
}

#endif