#include <iostream>
#include <memory>
#include <random>
#include <cassert>
#include <thread>
#include <algorithm>
#include <climits>
#include <malloc.h>
#include <csignal>

#include <cstddef>
#include <string>
#include <cstring>

#include "../include/Keys.h"
#include "../include/arena.h"

using namespace Orbit;
using namespace std;


// Bug -> is not because of comparing function, must be from the sort

Slice save;
Slice save1;

struct Comparator{

    // if <= 0 then it a is <= b
    bool operator()(const Slice &a, const Slice &b) const{
        save = a;
        save1 = b;
        return (CompareSlices(a,b) <= 0);
    }

};

// Basicvally sort is causing the size of a Slice to grow exponentially and for soem reason sets a sata_ to a nullpointer somehow, but its not because of the Comaprator function


const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 "; // _/?><.,#+=[]-_!@$%^&*()|`~{};:\\\"
std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> dist(0, charset.size() - 1);
std::uniform_int_distribution<int> lengthDist(1, 126);

// basically a random device is like a library, the generator is the librarian and the uniofrm_distribution is the librarian getting books and calling the funciton is like passing the books

std::string generateRandomString() {

    int length = lengthDist(gen);
    std::string randomString;
    for (int i = 0; i < length; ++i) {
        randomString += charset[dist(gen)];
    }

    return randomString;
}


// here is the problem one of them is a nullptr
void signalHandler(int signum){
    cout << save.getSize() << endl;
    cout << save1.getSize() << endl;

    exit(signum);
}

int main(){

    string temp = string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");

    // Creating Slices

    size_t size = 63;
    // problem with the string constructor

    // don't need c null terminator because of fized size given
    char data[size] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

    Slice test = Slice();
    Slice testBytes = Slice(data, size);
    Slice testString = Slice(temp);

    // copy operator
    test = testString;

    Slice testParaCopy = Slice(testBytes);

    // helper functions
    test.clear();

    assert(test.isEmpty() && !testString.isEmpty());

    for(int i =0; i < size; i++){
        assert(testBytes[i] == data[i]);
    }

    assert(testParaCopy.Print() == "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");



    // equality operator
    assert(testString == testBytes);
    assert(testParaCopy != test);

    // Arena creation for memory
    Arena arena = Arena();

    size = 1000000;

    vector<Slice> testSet(size);
    vector<string> actual(size);



    signal(SIGSEGV, signalHandler);


    for(int i = 0; i < size; i++){

        // here actual won't go out of scope
        string temp = generateRandomString();
        char* ptr = arena.Allocate(temp.size());

        memcpy(ptr, temp.c_str(), temp.size());

        actual[i] = temp;
        testSet[i] = Slice(ptr,temp.size());

        assert(testSet[i].Print() == actual[i]);


    }

    for(int i = 0; i < size - 1; i++){
        CompareSlices(testSet[i], testSet[i+1]);
        CompareSlices(testSet[i], testSet[i]);
        CompareSlices(testSet[i + 1], testSet[i]);
        CompareSlices(testSet[i + 1], testSet[i+1]);

        assert(testSet[i].Print() == actual[i]);
        assert(testSet[i].getSize() < 128);
        assert(testSet[i].getDataPtr() != nullptr);

    }

    cout << "Reaches first checkpoint" << endl;

    cout << arena.getMemorySize() << endl;

    // data is going out of scope because we are sorting it - need to use arena for memory mangement
    sort(testSet.begin(), testSet.end(), Comparator());

    cout << "Reaches second checkpoint" << endl;
    sort(actual.begin(), actual.end());


    // degmentation fault accessing memory it shouldn't
    for(int i = 0; i < size; i++){
        assert(testSet[i].Print() == actual[i]);
    }

    return 0;
}

/*
        cout << "Test: " << (testSet[i].Print()) << endl;
        cout << "Actual: " << actual[i] << endl << endl;;
*/


/*

Case 1

Beginning Allocation
Allocating new Block
munmap_chunk(): invalid pointer
Aborted (core dumped)


Case 2

Beginning Allocation
End Allocation

munmap_chunk(): invalid pointer
Aborted (core dumped)

Case 3

Beginning Allocation
End Allocation

Reaches the end
munmap_chunk(): invalid pointer
Aborted (core dumped)

Case 4

Beginning Allocation
Allocating new Block
Allocating Successfull
End Allocation

munmap_chunk(): invalid pointer
Aborted (core dumped)


*/