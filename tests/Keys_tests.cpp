#include "../include/Keys.h"

#include <random>
#include <iostream>
#include <iomanip> 
#include <vector>

using namespace Orbit;
using namespace std;

int main(){

    // assuming key and value can be as large as possible - later can limit key size
    int minLength = 1;
    int maxLength = 3419; // 507

    const string PossibleCharacthers = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789// _/?><.,#+=[]-_!@$%^&*()|`~{};:\\\"";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, PossibleCharacthers.size() - 1);
    std::uniform_int_distribution<int> lengthDist(minLength, maxLength);


    int size = 100000;
    // max stack size is 8192
    char buffer[7000];
    char* ptr = buffer;

    for(int i = 0; i < size; i++){

        int length = lengthDist(gen);
        std::string key;
        for (int _ = 0; _ < length; ++_) { key += PossibleCharacthers[dist(gen)]; }

        length = lengthDist(gen);
        std::string value;
        for (int _ = 0; _ < length; ++_) { value += PossibleCharacthers[dist(gen)]; }


        UserRequest input(Slice(key), Slice(value),1,i);


        encode(input, ptr);
        UserRequest output = decode(ptr);

        // when the size is 128 we get 0?
        //cout << input.getKey().getSize() << " vs " << output.getKey().getSize() << endl;
        //cout << input.getValue().getSize() << " vs " << output.getValue().getSize() << endl;

        assert(input.getKey() == output.getKey());
        assert(input.getValue() == output.getValue());
        assert(input.getType() == output.getType());
        assert(input.getSequenceNumber() == output.getSequenceNumber());


    }

    cout << "Succesfully passed test case" << endl;

   /* !!!! Problem in the length encode its something is going wrong, only ones where the input is split into multiple bytes breaks !!!*/

    return 0;
}

/*

        for(int i = 0; i < length; i++){
            cout <<  std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(ptr[i])) << " ";
        }

 size_t length = input.getLength();
    cout <<" Length of User Input: " << length << endl;

    for(int i = 0; i < length; i++){
        cout <<  std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(ptr[i])) << " ";
    }

    cout << std::dec << endl;
        cout << output.getKey().Print() << " " << output.getValue().Print() << " " << static_cast<unsigned int>(output.getType()) << " " << output.getSequenceNumber() << endl;

*/