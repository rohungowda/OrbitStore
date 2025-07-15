#ifndef MEMTABLE_H
#define MEMTABLE_H

#include "Keys.h"
#include "arena.h"

// have the key as a hash converted and serialzied, right now think about it as simple key and values

namespace Orbit{



    class MemTable{
        public:
            MemTable();
            //memtable(uint64_t startSequenceNumber, Arena * allocate);

            /*
            So the idea is make this memtable inherit from enable_shared_from_this that way we can transfer ownership very easily
            essentiaslly here is what happens
            user makes a GET and PUT request -> 
            Check Size using the getMemorySize()
            if memory >= limit -> block request

            {Need to figure out how to transfer ownership -> first to the distribrutor then to any threads that were set with the old one }

            */
            ~MemTable();

            UserRequest GET(UserRequest req);
            void PUT(UserRequest req);

            size_t getMemorySize() {return allocater.getMemorySize();}
        

        private:
            Arena allocater;
            //static atomic<uint64_t> sequenceNumber; // Keep it in the manager thread which directs everything
            vector<char*> TempDataStructure;
            const size_t limit = 8192; // 1024 * 8, so memtable should have enough data for 8 pages of arene memory for now
            mutex MemTable_mtx; // rememer right now we are doign monolithic locking, changing to either fine grained lockign or even better wait-free locking

    };

};

#endif