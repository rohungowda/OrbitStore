#ifndef ARENA_H
#define ARENA_H

#include <cstddef>
#include <vector>
#include <atomic>
#include <mutex>
#include "LinkedList.h"

#include <iostream>
using namespace std;

namespace Orbit{
    class Arena{
        public:
            Arena();

            ~Arena();

            // Allocates some amount of memory needed
            char * Allocate(size_t size);

            // get memory size
            size_t getMemorySize();


            void print(){
                const Node* root = freespace.getRoot();
                
                while(root != nullptr){
                    root->Print();
                    cout  << " | " << root->dataSize << endl;
            
                    root = root->next;
                }

                cout << freespace.counter << endl;
            }

        private:
            char* ptr;
            size_t bytesRemain;
            std::vector<char*> blockHolder;

            std::atomic<size_t> totalSize;
            std::mutex mtx;
            const size_t BLOCKSIZE = 1024;
            
            LinkedList freespace;

            // allocates a large memory block
            char * AllocateBlock(size_t size);

            // allocate aligned
            char* AllocateAligned(size_t size);

            

    };
}

#endif