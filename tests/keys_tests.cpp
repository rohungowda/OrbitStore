#include <iostream>
#include <memory>
#include <random>
#include <cassert>
#include <thread>

#include "../include/Keys.h"

using namespace Orbit;
using namespace std;

int main(){

    // Creating Slices

    size_t size = 12;

    char data[size] = "hello world";

    Slice test = Slice();
    Slice testBytes = Slice(data, size);
    Slice testString = Slice(string("hello world"));

    // copy operator
    test = testString;
    Slice testParaCopy = Slice(testBytes);

    
    // helper functions
    test.clear();
    // copy does shallow copy
    assert(test.isEmpty() && testString.isEmpty());

    for(int i =0; i < 12; i++){
        assert(testBytes[i] == data[i]);
    }

    testParaCopy.Print();

    return 0;
}