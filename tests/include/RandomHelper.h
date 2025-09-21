#ifndef RANDOMHELPER_H
#define RANDOMHELPER_H

#include <random>

using namespace std;

namespace Orbit{

    class RandomHelper{
        public:
            RandomHelper(int minimumLength, int maximumLength) 
            : gen(rd()), dist(0, PossibleCharacthers.size() - 1), lengthDist(minimumLength, maximumLength)
            {

            }

            string randomStringGet();

        private:
            const string PossibleCharacthers = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789// _/?><.,#+=[]-_!@$%^&*()|`~{};:\\\"";
            std::random_device rd;
            std::mt19937 gen;
            std::uniform_int_distribution<int> dist;
            std::uniform_int_distribution<int> lengthDist;
    };

}


#endif