#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <cstddef>
#include <string>
#include <cstring>
#include <cassert>

namespace Orbit{
    
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

    class UserRequest{
        public:
            UserRequest(Slice key, Slice value, uint8_t type, uint64_t sequenceNumber)
            : key_(key), value_(value), type_(type), sequenceNumber_(sequenceNumber) {}

            bool encode(const char* dest){
                
            }

            size_t getLength(){
                // TODO keyLength + actual key + SequenceNumber + type + valueLength + value
                return 0;
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