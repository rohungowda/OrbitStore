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

            // Allocates some amount of memory needed and return a pointer to the mnemory
            char * Allocate(size_t size);

            // get memory size
            size_t getMemorySize();

            // prints out information on the Linked List
            void print(){
                const Node* root = freespace.getRoot();
                

                while(root != nullptr){
                    cout  << root->dataSize << " -> ";
                    root = root->next;
                }
                cout <<  " NULL" << endl;
                cout <<"Number of Unused Bytes: " << freespace.counter << endl;
            }

        private:

            // Local Variables for Block
            char* ptr;
            size_t bytesRemain;

            // Block holder array ( Could use Linked List )
            std::vector<char*> blockHolder;

            // totalSize
            std::atomic<size_t> totalSize;
            
            // mutex for block
            std::mutex mtx;
            const size_t BLOCKSIZE = 1024;
            
            // captures freespace that can be used instead of just the block **Might need to handle multithreading?
            LinkedList freespace;

            // allocates a large memory block of BlockSize
            char * AllocateBlock(size_t size);

            // TODO Implement
            char* AllocateAligned(size_t size);

            

    };
}

#endif