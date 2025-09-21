#include "../include/Keys.h"
#include "include/RandomHelper.h"

#include <random>
#include <iostream>
#include <iomanip> 
#include <vector>

using namespace Orbit;
using namespace std;

class TestKeys{

    public:


        TestKeys(int size) 
        : helper(RandomHelper(0,size))
        {

        }

        void TestAsteroidsConstructors(int numberTests){
            Asteroid empty = Asteroid();
            assert((empty.getSize() == 0) && (empty.getDataPtr() == nullptr));

            for(int i = 0; i < numberTests; i++){
                string randomString = helper.randomStringGet();
                empty = Asteroid(randomString);
                assert((empty.getSize() == randomString.length()) && (empty.Print() == randomString));

                randomString = helper.randomStringGet();
                const char* randomPointer = randomString.c_str();
                empty = Asteroid(randomPointer, randomString.length());

                assert((empty.getSize() == randomString.length()) && (empty.Print() == randomString));
            }


        }



    private:
        RandomHelper helper;

};


int main(){

    TestKeys tester = TestKeys(100);

    tester.TestAsteroidsConstructors(25);

    cout << "Successfully Passed All Tests" << endl;

    return 0;
}
