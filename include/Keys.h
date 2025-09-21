#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <cstddef>
#include <string>
#include <cstring>
#include <cassert>



#include <iostream>
#include <iomanip> 

using namespace std;

namespace Orbit{

    // this is a representation of a piece of data
    class Asteroid{
        public:
            Asteroid();

            // Take in a string : key, JsonObject, Value
            Asteroid(const std::string &data);

            // takes in a const char* data
            Asteroid(const char* data, size_t n);

            // copyable - does not make a shallow copy
            Asteroid(const Asteroid&) = default;
            Asteroid& operator=(const Asteroid&) = default;

            //const getter function for size of byte data
            size_t getSize() const { return size;}

            // return immutable pointer to data
            const char* getDataPtr() const { return data_;}

            // clear data
            void clear(){
                data_ = nullptr;
                size = 0;
            }

            // Check data is empty both in size and nullpointer
            bool isEmpty(){ return (size == 0) && (data_ == nullptr);}

            // make sure i < size and returns a data[i] value from pointer
            char operator[](size_t i) const {
                assert(i < size);
                return data_[i];
            }

            // Print out data to string and makes a contract, stating it will not change the value with the this object
            std::string Print() const {return std::string(data_,size); }

            // 0 if this == other
            // > 0 if this > other
            // < 0 if this < other
            int CompareSlices(const Asteroid other);


        private:
            const char* data_;
            size_t size;
    };

    // inline replace function call with function body, will save memory and time
    inline bool operator==(const Asteroid& x, const Asteroid& y){
        return ((x.getSize() == y.getSize()) && (std::memcmp(x.getDataPtr(), y.getDataPtr(), x.getSize()) == 0));
    }

    inline bool operator!=(const Asteroid x, const Asteroid y){
        return !(x == y);
    }


    class LengthEncoding{

        public:
            const static int CHARBYTESIZE = 128; // 1000 0000
            const static int BYTESIZE = 0x80; // 1000 0000
            const static int ByteMask = 0x7F; // 1111 1111

            // how many bytes it takes to store a size variable while using the continuation bit encoding
            static int LengthCalculate(size_t SizeOfBytes);

            // Will Decode Length and move the pointer reference down
            static int DecodeLength(char* &encoding);

            static char* EncodeLength(char* pointer, const Asteroid &data);

    };



    enum Operation{
        GET, // key
        PUT, // key value
        DELETE, // key
        SCAN // pattern
    };

    enum Status{
        SUCCESS,
        FAIL
    };


 class InternalRecord{
        public:
            InternalRecord(Asteroid key, Asteroid value, uint64_t sequenceNumber);

            // as long EncodingPointer doesn't change
            InternalRecord(char* encodingPointer);

            Asteroid getKey();

            Asteroid getValue();

            uint64_t getSequenceNumber();


        private:

            Asteroid getDataFromPointer(char* dataPointer);

            char* keyPointer;
            char* valuePointer;
            char* sequencePointer;

    };

    // atomic global sequence
    class UserRequest{
        public:
            UserRequest(string key, string value, uint64_t sequenceNumber, Operation Op_, Status status_)
            : record(InternalRecord(Asteroid(key), Asteroid(value), sequenceNumber)), Op(Op_), status(status_)
            {

            }

        private:
            Status status;
            InternalRecord record;
            Operation Op;
        };


}

#endif