#include <iostream>
#include <memory>
#include <random>
#include <cassert>
#include <thread>

#include "../include/arena.h"


using namespace std;
using namespace Orbit;


void test_ByteAllocation(Arena &copy){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1,1023);


    int totalSize = 0;

    // Due to 2s compliment the largest value that can be stored per byte is 127
    for(int i = 0; i < 127; i++){

        int Size = dist(gen);
        char* store = copy.Allocate(Size);

        // value test
        for(int j = 0; j < Size; j++){
            *(store + j) = (static_cast<char>(i+1));
        }

        for(int j = 0; j < Size; j++){
            assert(((int)*(store + j)) == (i+1));
        }

    }

}

int main(){

    // Variable Intialization
    Arena testInstance = Arena();

    vector<thread> testThreads;

    for(int i = 0; i < 10; i++){
        testThreads.push_back( thread(test_ByteAllocation, ref(testInstance)) );
    }
  


    for (auto& t : testThreads) {
        t.join();
    }

    cout << "Passed All Test Cases for Arena with 10 threads" << endl;

    return 0;
}