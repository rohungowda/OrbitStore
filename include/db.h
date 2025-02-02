#ifndef DB_H
#define DB_H

#include <string>

namespace Orbit{

struct UserRequest{};

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

        // singleton want to keep internals hidden

        // Basic Operations
        virtual STATUS PUT(UserRequest req) = 0;
        virtual STATUS GET(UserRequest req) = 0;
        virtual STATUS DELETE(UserRequest req) = 0;
        virtual STATUS SCAN(UserRequest req) = 0; // modify delete or read a specific range of keys
        
        virtual STATUS BatchRequest(UserRequest req) = 0;

        // not sure yet
        virtual STATUS DBmetrics() = 0;

        virtual STATUS deleteDB() = 0;
        virtual STATUS closeDB() = 0;

    private:
        virtual STATUS runCompaction() = 0;
        virtual STATUS start() = 0;

};

}


#endif