#include "../include/LinkedList.h"
#include <iostream>
#include <vector>

using namespace Orbit;
using namespace std;

int main(){

    LinkedList freespace = LinkedList();

    vector<string> data;

    string temp = "";

    for(int i = 0; i < 10; i++){

        temp += to_string(i);
        data.push_back(temp);

    }

    for(int i = 0; i < 10; i++){

        freespace.createEndNode(data[i].data(), data[i].size());
    }

    vector<int> space = {5,3,1,2,5,4,3,2,1,5,6,7,4,3};

    for(int c : space){

        cout << "Finding free space for " << c << endl;

        char* ptr = freespace.findCandidate(c);

        if(ptr != nullptr){
            for(int j = 0; j < c; j++){
                ptr[j] = '*';
            }
        }
        
    
        for(int i = 0; i < 10; i++){
            cout << data[i] << " | ";
        }
        cout << endl;
    
        const Node* root = freespace.getRoot();
    
        while(root != nullptr){
            cout << root->dataSize << " | ";
    
            root = root->next;
        }

        cout << endl << endl;

    }

   


    return 0;
}