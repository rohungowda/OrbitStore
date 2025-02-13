#ifndef ARENA_H
#define ARENA_H

#include <cstddef>
#include <vector>
#include <atomic>
#include <mutex>


namespace Orbit{
    class Arena{
        public:
            Arena();

            ~Arena();

            // Allocates some amount of memory needed
            char * Allocate(size_t size);

            // get memory size
            size_t getMemorySize();

        private:
            char* ptr;
            size_t bytesRemain;
            std::vector<char*> blockHolder;

            std::atomic<size_t> totalSize;
            std::mutex mtx;
            const size_t BLOCKSIZE = 1024;
            
            std::vector<char*> freeSpace;

            // allocates a large memory block
            char * AllocateBlock(size_t size);

            // allocate aligned
            char* AllocateAligned(size_t size);

            

    };
}

#endif