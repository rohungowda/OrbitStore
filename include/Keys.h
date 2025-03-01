#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <cstddef>
#include <string>
#include <cstring>
#include <cassert>

#include <iostream>

namespace Orbit{
    
    // TODO - Testing + Assertion + Helper functions

    static int CHARBYTESIZE = 128;
    static int BYTESIZE = 256;
    static int ByteMask = 0x7F;

    class Slice{
        public:
            Slice(): data_(nullptr), size(0) {}
            Slice(const std::string &data) : data_(data.data()), size(data.size()) {}


            Slice(const char* data, size_t n) : data_(data), size(n) {}

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

            // comparator

        private:
            // data is immutable once assigned
            const char* data_;
            size_t size;
    };

    // == operator
    bool operator==(const Slice& x, const Slice& y){
        return ((x.getSize() == y.getSize()) && (std::memcmp(x.getDataPtr(), y.getDataPtr(), x.getSize()) == 0));
    }

    bool operator!=(const Slice& x, const Slice& y){
        return !((x.getSize() == y.getSize()) && (std::memcmp(x.getDataPtr(), y.getDataPtr(), x.getSize()) == 0));
    }

    // 0 if equal
    // > 0 if x > y
    // < 0 if x < y
    // note smaller values should be sorted first and punctuation should come first
    int CompareSlices(const Slice& x, const Slice& y){
        const size_t size = std::min(x.getSize(), y.getSize());

        // there is something going on here where one of the size and pointers are not being copied properly?
        
        int compared = std::memcmp(x.getDataPtr(), y.getDataPtr(), size);

        if(compared == 0){
            return (x.getSize() == y.getSize()) ? compared : (x.getSize() < y.getSize()) ? -1 : 1;
        }else{
            return compared;
        }

    }

    // how many bytes it takes to store a size variable while using the continuation bit encoding
    int LengthCalculate(size_t size){
        int bytes = 1;
        while(size >= CHARBYTESIZE){
            size >>= 7;
            bytes += 1;
        }

        return bytes;
    }

    // char - > repurpose the MSB as a continuation bit
    // const char* not planning on modifying data
    // char* plainning on modifying data
    // returns a char pointing to the next part of the sequence
    char* createLengthEncoding(char * locate, const Slice &value){

        char* reader = locate;

        size_t keyByteSize = LengthCalculate(value.getSize());
        size_t actualDataSize = value.getSize();


        // this is for size_t which is always positive
        while(keyByteSize > 0){
            *(reader++) = (keyByteSize & ByteMask) | BYTESIZE;
            keyByteSize >> 7;
        }

        // sets the MSB or continution bit to 0
        reader--;
        *(reader) = *(reader) & ByteMask;
        reader++;


        for(size_t i = 0; i < value.getSize(); i++){
            *(reader++) = *(value.getDataPtr() + i);
        }

        return reader;

    }



    class UserRequest{
        public:
            UserRequest(Slice key, Slice value, uint8_t type, uint64_t sequenceNumber)
            : key_(key), value_(value), type_(type), sequenceNumber_(sequenceNumber) {}


            size_t getLength(){
                // keyLength + actual key + SequenceNumber + type + valueLength + value
                return (LengthCalculate(key_.getSize()) + key_.getSize() + sizeof(uint8_t) + sizeof(uint64_t) + LengthCalculate(value_.getSize()) + value_.getSize());
            }

            // getters
            Slice getKey() const{ return key_; }
            Slice getValue() const{ return value_; }
            uint8_t getType() const{ return type_; }
            uint64_t getSequenceNumber() const{ return sequenceNumber_; }


        private:
            Slice key_;
            Slice value_;
            uint8_t type_;
            uint64_t sequenceNumber_;
    };

    void encode(UserRequest req, char* dest){
        // key encoding
        dest = createLengthEncoding(dest, req.getKey());
        
        // type encoding
        *(dest++) = static_cast<char>(req.getType());

        // seqeuence encoding
        uint64_t copy = req.getSequenceNumber();

        // want it to be 8 bytes exactly
        for(int i = 0; i < 8; i++){
            *(dest++) = copy & 0xFF;
            copy >>= 8;
        }

        // value encoding
        dest = createLengthEncoding(dest, req.getValue());

        // need a check here to make sure everything works -> like an assert

    }


}

#endif