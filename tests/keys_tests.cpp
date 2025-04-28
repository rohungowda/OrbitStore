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

struct Comparator{

    // if <= 0 then it a is <= b
    bool operator()(const Slice &a, const Slice &b) const{
        return (CompareSlices(a,b) <= 0);
    }

};


const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 "; // _/?><.,#+=[]-_!@$%^&*()|`~{};:\\\"
std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<int> dist(0, charset.size() - 1);
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

// 8192 KB of stack Space, max amount of memory I can use for the stack
int main(){

    // Arena creation for memory
    Arena arena = Arena();

    int size = 100000;


    vector<string> actual;
    vector<Slice> test;


/*
TODO***

- Combine Arena into LinkedList
 - Linked List must be able to traverse through Nodes, insert nodes, and delete nodes
- the Node class will Create Buffers, check buffers, fill bufffers, resetr buffers
- the arena class will put all of it together


*/


/*
Tests Passed

- able to sucessfully create Slice objects that retain where the data is on the heap
- sorting error was really happening bercause of how a sort will call a recusion which increases the stack size
- pushing a string onto the vector as a refernce may mean we lose the actual string that was created
- figuree out wy there is a mysterious zero (allocation always did an inital block first and needed to make sure the nodes that were added were for a larger limit)
- data doesn't print becasue the actual root node doesn't ahve anything to priont, its empoty hgeap space that hasn't been used yet

*/



    for(int i = 0; i < size; i++){

        string temp = generateRandomString();
        char* ptr = arena.Allocate(temp.size());

        memcpy(ptr, temp.data(), temp.size());

        actual.push_back(temp);

        // intializes a pointer characther to where ptr data is
        test.push_back(Slice(ptr, temp.size()));


    }

    cout << "Loads everything into the vectors" << endl;


    // cause of the problem is here when I sort, technically don't need to sort becasue storing sorteed structure, but want to understand why - !! Recursive calls lead to stack overflow
    //sort(test.begin(), test.end(), Comparator());
    //sort(actual.begin(), actual.end());

    cout << "Succesfully sorts vectors" << endl;

    for(int i = 0; i < size; i++){
        assert(test[i].Print() == actual[i]);
        assert(test[i].getSize() < 128);

    }


    
    arena.print();
    cout << endl << (arena.getMemorySize()) << endl;
    

    cout << "Succesfully passes all tests" << endl;

    return 0;
}