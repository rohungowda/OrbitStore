#include "../include/Keys.h"

using namespace std;
using namespace Orbit;


Asteroid::Asteroid(): data_(nullptr), size(0) {}
Asteroid::Asteroid(const std::string &data) : data_(data.data()), size(data.size()) {}
Asteroid::Asteroid(const char* data, size_t n) : data_(data), size(n) {}

int Asteroid::CompareSlices(const Asteroid other){

    const size_t size = std::min(this->getSize(), other.getSize());
    int compared = std::memcmp(this->getDataPtr(), other.getDataPtr(), size);

    if(compared == 0){
        return (this->getSize() == other.getSize()) ? compared : (this->getSize() < other.getSize()) ? -1 : 1;
    }else{
        return compared;
    }

}

int LengthEncoding::LengthCalculate(size_t SizeOfBytes){
    int bytes = 1; // everything must be stored in 1 byte, regardless
    while(SizeOfBytes >= CHARBYTESIZE){
        SizeOfBytes >>= 7;
        bytes += 1;
    }

    return bytes;
}

int LengthEncoding::DecodeLength(char* &encoding){

    int continuation_bit = 1;
    int length = 0;
    int i = 0;
    int move = 0;

    while(continuation_bit){
        continuation_bit = (*encoding) & BYTESIZE; // continuation bit

        length |= ((*(encoding++) & ByteMask) << (move)); // get length from encoding
        i += 1;
        move = ((8 * (i)) - 1);

    }

    return length;

}

char* LengthEncoding::EncodeLength(char* pointer, const Asteroid &data){

    size_t actualDataSize = data.getSize(); // size of data in bytes
    size_t lengthSize = LengthCalculate(actualDataSize); // how many bytes it will take to store data

    while(lengthSize-- > 0){
        *(pointer++) = (actualDataSize & ByteMask) | BYTESIZE;
        actualDataSize >>= 7; 
    }

    std::memcpy(pointer, data.getDataPtr(), data.getSize());
    pointer += data.getSize();

    return pointer;

}

InternalRecord::InternalRecord(Asteroid key, Asteroid value, uint64_t sequenceNumber){
    int size = LengthEncoding::LengthCalculate(key.getSize()) 
            + LengthEncoding::LengthCalculate(value.getSize())
            + sizeof(uint64_t);
                
    char * temporaryPointer = new char[size];

    keyPointer = temporaryPointer;
    temporaryPointer = LengthEncoding::EncodeLength(temporaryPointer,key);

    valuePointer = temporaryPointer;
    temporaryPointer = LengthEncoding::EncodeLength(temporaryPointer,value);

    sequencePointer = temporaryPointer;
    std::memcpy(temporaryPointer, &sequenceNumber, sizeof(uint64_t));
}

InternalRecord::InternalRecord(char* encodingPointer){

    keyPointer = encodingPointer;
    
    int keySize = LengthEncoding::DecodeLength(encodingPointer);
    encodingPointer += keySize;

    valuePointer = encodingPointer;
    int valueSize = LengthEncoding::DecodeLength(encodingPointer);
    encodingPointer += valueSize;

    sequencePointer = encodingPointer;
}

Asteroid InternalRecord::getDataFromPointer(char* dataPointer){
    int size = LengthEncoding::DecodeLength(dataPointer);
    return Asteroid(dataPointer, size);
}

Asteroid InternalRecord::getKey(){
    return getDataFromPointer(keyPointer);
}

Asteroid InternalRecord::getValue(){
    return getDataFromPointer(valuePointer);
}

uint64_t InternalRecord::getSequenceNumber(){
    uint64_t sequenceNumber = 0;
    std::memcpy(&sequenceNumber, sequencePointer, sizeof(uint64_t));
    return sequenceNumber;
}