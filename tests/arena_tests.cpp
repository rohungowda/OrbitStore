#include <iostream>
#include <fstream>
#include <memory>
#include <random>
#include <cassert>
#include <thread>
#include <future>

#include "../include/arena.h"

using namespace std;
using namespace Orbit;



void MonolithicTest(int numberThreads, int numberRequests, int minLength=1, int maxLength=1024){

    Arena arena = Arena();
    ofstream file("output.txt");

    const string PossibleCharacthers = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789// _/?><.,#+=[]-_!@$%^&*()|`~{};:\\\"";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, PossibleCharacthers.size() - 1);
    std::uniform_int_distribution<int> lengthDist(minLength, maxLength);

    int averageWorload = numberRequests / numberThreads;
    int remainder = (numberRequests % numberThreads);

    vector<vector<string>> data(numberThreads,vector<string>());
    vector<future<vector<pair<char*,size_t>>>> futures;
    vector<pair<char*,size_t>> check;


    for(int i = 0; i < numberThreads; i++){
        for(int j = 0; j < averageWorload; j++){

            int length = lengthDist(gen);
            std::string randomString;
            for (int _ = 0; _ < length; ++_) { randomString += PossibleCharacthers[dist(gen)]; }
            data[i].push_back(randomString);

        }

        std::future<vector<pair<char*,size_t>>> result = std::async(std::launch::async, [&arena, &threadData = (data[i])](){
            
            vector<pair<char*,size_t>> pointers;
            for(int i = 0; i < threadData.size(); i++){
                char* ptr = arena.Allocate(threadData[i].size());
                
                // carefull of null bytes
                memcpy(ptr, threadData[i].data(), threadData[i].size());
                pointers.push_back({ ptr, threadData[i].size() });
            }

            return pointers;
        });

        // move ownership of future freom result to the index at futures
        futures.push_back(move(result));
    }

    cout << "All Threads Processing" << endl;

    for(int i = 0; i < numberThreads; i++){
        vector<pair<char*,size_t>> temp = futures[i].get();
        check.insert(check.end(),temp.begin(), temp.end());

        for(int j = 0; j < data[i].size(); j++){
            file << data[i][j] << endl;
        }

    }

    // delete all vector data here except pointers - make sure there is no data corruption

    for(int i = 0; i < data.size(); i++){
        data[i].clear();
    }
    data.clear();
    futures.clear();
    file.close();

    // check that poitners match file values

    ifstream InFile("output.txt");

    string line;
    int index = 0;
    while (getline(InFile, line)) {
        assert(line == string(check[index].first, check[index].second));
        index += 1;
    }

    cout << "All tests Passed Sucessfuly !" << endl;
    cout << "Bytes lost + remaining bytes in LL" << endl;
    arena.print();

}

int main(){
    MonolithicTest(500,100000);
}