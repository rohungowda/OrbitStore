#ifndef DB_H
#define DB_H

#include <string>

#include "Conversions.h"

namespace Orbit{

enum STATUS{

};

struct Properties{

};

// make DB a singleton access?
class DB{
    public:
        DB(const Properties &props, const std::string &DBname);

        DB(const DB&) = delete;
        DB operator=(const DB&) = delete;

        STATUS start();

        // Basic Operations
        STATUS PUT(UserRequest req);
        STATUS GET(UserRequest req);
        STATUS DELETE(UserRequest req);
        STATUS SCAN(UserRequest req); // modify delete or read a specific range of keys
        
        STATUS BatchRequest(UserRequest req);

        // not sure yet
        STATUS DBmetrics();

        STATUS deleteDB();
        STATUS closeDB();

    private:
       STATUS runCompaction();

};

}


#endif