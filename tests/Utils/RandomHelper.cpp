#include "../include/RandomHelper.h"

using namespace Orbit;

string RandomHelper::randomStringGet(){
    int length = lengthDist(gen);
    char* array = new char[length];

    for (int _ = 0; _ < length; ++_) { array[_] += PossibleCharacthers[dist(gen)]; }

    return string(array);
}
