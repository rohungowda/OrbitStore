#include "../include/memtable.h"

#include <random>
#include <iostream>
#include <iomanip> 
#include <vector>

using namespace Orbit;
using namespace std;

int main(){

    MemTable table = MemTable();
    table.PUT(UserRequest(Slice("Hello"), Slice("World"),1,0));

    cout << table.GET(UserRequest(Slice("Hello"), Slice(),0,0)).toString() << endl;

    return 0;
}