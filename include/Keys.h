#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <cstddef>
#include <string>
#include <cstring>
#include <cassert>



#include <iostream>
#include <iomanip> 



namespace Orbit{

    static int CHARBYTESIZE = 128;
    static int BYTESIZE = 0x80;
    static int ByteMask = 0x7F;


    static int TypeMove = 2;
    static uint64_t SEQUENCE_MASK = 0xFFFFFFFFFFFFFFFC;
    static uint64_t OPERATION_MASK = 0x03;

    // this is the data representation in 
    class Slice{
        public:
            Slice();

            Slice(const std::string &data);

            Slice(const char* data, size_t n);

            // copyable - does not make a shallow copy
            Slice(const Slice&) = default;
            Slice& operator=(const Slice&) = default;

            //const getter functions
            size_t getSize() const { return size;}

            const char* getDataPtr() const {
                return data_;
            }

            void clear(){
                data_ = nullptr;
                size = 0;
            }

            bool isEmpty(){
                return (size == 0);
            }

            char operator[](size_t i) const {
                assert(i < size);
                return data_[i];
            }

            std::string Print() const {return std::string(data_,size); }


        private:
            const char* data_;
            size_t size;
    };

    // == operator
    inline bool operator==(const Slice& x, const Slice& y){
        return ((x.getSize() == y.getSize()) && (std::memcmp(x.getDataPtr(), y.getDataPtr(), x.getSize()) == 0));
    }

    inline bool operator!=(const Slice& x, const Slice& y){
        return !(x == y);
    }

    // 0 if equal
    // > 0 if x > y
    // < 0 if x < y
    inline int CompareSlices(const Slice& x, const Slice& y){
        const size_t size = std::min(x.getSize(), y.getSize());
        
        int compared = std::memcmp(x.getDataPtr(), y.getDataPtr(), size);

        if(compared == 0){
            return (x.getSize() == y.getSize()) ? compared : (x.getSize() < y.getSize()) ? -1 : 1;
        }else{
            return compared;
        }

    }

    // how many bytes it takes to store a size variable while using the continuation bit encoding
    inline int LengthCalculate(size_t size){
        int bytes = 1;
        while(size >= CHARBYTESIZE){
            size >>= 7;
            bytes += 1;
        }

        return bytes;
    }

    inline int getLengthFromEncoding(char* data){

        int bit = 1;
        int VarSize = 0;

        // int can only take up to 4 bytes

        int i = 0;
        int move = 0;

        while(bit){
            bit = (*data) & BYTESIZE;

            VarSize |= ((*(data++) & ByteMask) << (move));
            i += 1;
            move = ((8 * (i)) - 1);

        }

        return VarSize;

    }

    // char - > repurpose the MSB as a continuation bit
    // const char* not planning on modifying data
    // char* plainning on modifying data
    // returns a char pointing to the next part of the sequence
    inline char* createLengthEncoding(char* locate, const Slice &value){

        char* reader = locate;

        size_t keyByteSize = LengthCalculate(value.getSize());
        size_t actualDataSize = value.getSize();


        // this is for size_t which is always positive
        // size -> extract all relevant
        // Little Endian = LSB .... MSB
        while(keyByteSize > 0){
            *(reader++) = (actualDataSize & ByteMask) | BYTESIZE;
            actualDataSize >>= 7;
            keyByteSize -= 1;
        }

        // sets the MSB or continution bit to 0
        reader--;
        *(reader) = *(reader) & ByteMask;
        reader++;

        // copy all data fromt eh value to the location identified
        std::memcpy(reader, value.getDataPtr(), value.getSize());

        // not really needed but I think its good
        reader += value.getSize();

        return reader;

    }


    enum Operation{
        READ,
        WRITE,
        DELETE
    };

    enum Status{
        SUCCESS,
        FAIL
    };

    // make the sequence number an atomic global variable acessible by all threads so that each request will have a monotically incrasing sequence number
    class UserRequest{
        public:

            UserRequest(Slice key, Slice value, Operation op) : key_(key), value_(value), op_(op) {}


            size_t getLength(){ return key_.getSize() + value_.getSize();}

            // getters
            Slice getKey() const{ return key_; }
            Slice getValue() const{ return value_; }


            std::string toString() {
                return std::string("User Request: { ") + " Key: " + this->key_.Print() + ", Value: " + this->value_.Print() + " }";
            }


        private:
            Slice key_;
            Slice value_;
            Operation op_;
        };




    class InternalKey{
        public:
            InternalKey(char* scratch, Slice key, Slice value, uint64_t sequenceNumber, Operation op){
                keyPtr_ = scratch;
                scratch = createLengthEncoding(scratch,key);

                valuePtr_ = scratch;
                scratch = createLengthEncoding(scratch,value);

                metadataPtr_ = scratch;
                sequenceNumber = (sequenceNumber << TypeMove) | op;
                std::memcpy(scratch, &sequenceNumber, sizeof(uint64_t));
            }

            InternalKey(char* data){
                keyPtr_ = data;
                size_t tempSize = getLengthFromEncoding(data);
                valuePtr_ = data + tempSize;
                data += tempSize;

                tempSize = getLengthFromEncoding(data);
                metadataPtr_ = data + tempSize;

            }

            Slice getKey(){
                char* temp = keyPtr_;
                int size = getLengthFromEncoding(temp);
                return Slice(temp);
            }

            Slice getValue(){
                char* temp = valuePtr_;
                int size = getLengthFromEncoding(temp);
                return Slice(temp);
            }

            uint64_t getSequenceNumber(){
                uint64_t sequenceNumber = 0;
                std::memcpy(&sequenceNumber, metadataPtr_, sizeof(uint64_t));
                return ((sequenceNumber & SEQUENCE_MASK) >> TypeMove);
            }
            
            Operation getOperation(){
                uint64_t sequenceNumber = 0;
                std::memcpy(&sequenceNumber, metadataPtr_, sizeof(uint64_t));
                return static_cast<Operation>(sequenceNumber & OPERATION_MASK);
            }


        private:
            char* keyPtr_;
            char* valuePtr_;
            char* metadataPtr_;

    };

    template<typename T>
    class DB_Response{
        public:
            DB_Response(Status s, T value) : s_(s), value_(value){}

        private:
        Status s_;
        T value_;

    };

}

#endif