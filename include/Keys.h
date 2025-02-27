#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <cstddef>
#include <string>
#include <cstring>
#include <cassert>

namespace Orbit{
    
    // TODO - Understasnd Bytes and char*, const char*, and what exactly I am storing + Finish Encoding Scheme and Helper functions + Test Everything built so far

    static int BYTESIZE = 128;

    class Slice{
        public:
            Slice(): data_(nullptr), size(0) {}
            Slice(const std::string &data) : data_(data.data()), size(data.size()) {}
            Slice(const char* data, size_t n) : data_(data), size(n) {}

            // copyable - but shallow copy copies the pointer and size not the actual data at pointer
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
    int CompareSlices(const Slice& x, const Slice& y){
        size_t size = std::min(x.getSize(), y.getSize());
        int compared = std::memcmp(x.getDataPtr(), y.getDataPtr(), size);

        if(compared == 0){
            return (x.getSize() == y.getSize()) ? compared : (x.getSize() < y.getSize()) ? 1 : -1;
        }else{
            return compared;
        }

    }

    int LengthCalculate(size_t size){
        int bytes = 1;
        while(size >= BYTESIZE){
            size >>= 7;
            bytes += 1;
        }

        return bytes;
    }

    // char* byte stores for onyl -128 to 128
    // unsigned char always stores in between 0 - 256 values
 
    char* createLengthEncoding(char * locate, const Slice &value){

        size_t keyByteSize = LengthCalculate(value.getSize());
        size_t actualKeySize = value.getSize();

        // this is for size_t which is always positive
        while(keyByteSize > 0){
            *(locate++) = actualKeySize && BYTESIZE;
            actualKeySize >> 7;
        }

        return nullptr;

    }

    class UserRequest{
        public:
            UserRequest(Slice key, Slice value, uint8_t type, uint64_t sequenceNumber)
            : key_(key), value_(value), type_(type), sequenceNumber_(sequenceNumber) {}

            bool encode(const char* dest){
                
            }

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

}

#endif