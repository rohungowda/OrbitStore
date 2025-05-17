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

    class Slice{
        public:
            Slice(): data_(nullptr), size(0) {}

            // creates a pointer to the string data but if the data doesn't exist it can't do anything
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

            // checks if the this slice starts with Slice x
            bool starts_with(const Slice &x){
                return (((size >= x.getSize())) && (memcmp(data_, x.getDataPtr(), x.getSize()) == 0));
            }


        private:
            // data is immutable once assigned but pointer can change where it points to
            const char* data_;
            size_t size;
    };

    // == operator
    bool operator==(const Slice& x, const Slice& y){
        return ((x.getSize() == y.getSize()) && (std::memcmp(x.getDataPtr(), y.getDataPtr(), x.getSize()) == 0));
    }

    bool operator!=(const Slice& x, const Slice& y){
        return !(x == y);
    }

    // 0 if equal
    // > 0 if x > y
    // < 0 if x < y
    int CompareSlices(const Slice& x, const Slice& y){
        const size_t size = std::min(x.getSize(), y.getSize());
        
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
    char* createLengthEncoding(char* locate, const Slice &value){

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


    // make the sequence number an atomic global variable acessible by all threads so that each request will have a monotically incrasing sequence number
    class UserRequest{
        public:
            UserRequest(Slice key, Slice value, uint8_t type, uint64_t sequenceNumber)
            : key_(key), value_(value), type_(type), sequenceNumber_(sequenceNumber) {}


            size_t getLength(){
                // keyLength + actual key + SequenceNumber + type + valueLength + value -> for this intial draft I am just going to include a byte but will change so can save space
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
        *(dest++) = static_cast<char>(req.getType()); // max value is 255 -> make sure to put it cast it back to unsigned char

        // seqeuence encoding
        uint64_t copy = req.getSequenceNumber();

        // strore number as little endian so that when it gets copied back to a uint_64_t number it automatically recognized the bytes in little endian and normally uses them
        std::memcpy(dest, &copy, sizeof(uint64_t));
        dest += 8;
       
        // value encoding
        dest = createLengthEncoding(dest, req.getValue());

    }

    UserRequest decode(char* ptr){
        int bit = 1;
        int keySize = 0;

        // int can only take up to 4 bytes

        int i = 0;
        int move = 0;

        while(bit){
            bit = (*ptr) & BYTESIZE;

            keySize |= ((*(ptr++) & ByteMask) << (move));
            i += 1;
            move = ((8 * (i)) - 1);

        }   


        Slice key(ptr, keySize);
        ptr+= keySize;
    

        uint8_t type = static_cast<unsigned char>(*(ptr++));

        uint64_t sequenceNumber = 0;

        std::memcpy(&sequenceNumber, ptr, sizeof(uint64_t)); // 8 bytes
        ptr += 8;

        i = 0;
        bit = 1;
        move = 0;
        int valueSize = 0;

        while(bit){
            bit = (*ptr) & BYTESIZE;

            valueSize |= ((*(ptr++) & ByteMask) << (move));
            i += 1;
            move = ((8 * (i)) - 1);
        }

        Slice value(ptr, valueSize);
        ptr+= valueSize;


        return UserRequest(key,value,type,sequenceNumber);

    }


}

#endif

/*

for (int j = bits - 1; j >= 0; --j) {
    printf("%d", (keySize >> j) & 1);
    if (j % 8 == 0) printf(" "); // add space every byte
}

printf("\n");

 for(int i = 0; i < 8; i++){
            *(dest++) = copy & 0xFF;
            copy >>= 8;
        }


        for(int i = 0; i < 7; i++){
            std::cout << sequenceNumber << std::endl;
            // &ptr automatically gets cast to an int -> allows for the number to be correcly represented
            sequenceNumber |= static_cast<unsigned char>(*(ptr++));
            sequenceNumber <<= 8;
        }

        sequenceNumber |= static_cast<unsigned char>(*(ptr++));
*/