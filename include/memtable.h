#ifndef MEMTABLE_H
#define MEMTABLE_H

#include "Keys.h"
#include "arena.h"

// have the key as a hash converted and serialzied, right now think about it as simple key and values
class InternalComparator;

class MemIterator;

namespace Orbit{

    class memtable{
        public:
            memtable(InternalComparator &compare);
            ~memtable();

            void GET(UserRequest req);
            void PUT(UserRequest req);
            void DELETE(UserRequest req);

            void SCAN(UserRequest req);
        
        private:
            Arena allocater;

    };

};

#endif