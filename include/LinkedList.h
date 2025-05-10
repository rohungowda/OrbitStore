#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include <cstddef>
#include <cstring>
#include <iostream>

namespace Orbit
{

    struct Node{

        Node() : pointer(nullptr), dataSize(0), next(nullptr) {}
        Node(char* ptr, size_t size, Node* next_) : pointer(ptr), dataSize(size), next(next_) {}
        Node(char* ptr, size_t size) : pointer(ptr), dataSize(size), next(nullptr) {}


        char* pointer;
        size_t dataSize;
        Node* next; 
    };



    class LinkedList
    {
    private:
        Node* root;
        Node* last;

    public:
        size_t limit = 5;

        int counter = 0;

        LinkedList();
        ~LinkedList();
        
        void createEndNode(char* ptr, size_t size);

        char* findCandidate(size_t size);

        // const before means you can change the variable holding the data but you can't modify ** needs to be reasdsinged variabel bc traverse!
        // const after means you can modify the data but the variable can't be reassigned
        const Node* getRoot() {return root;}


    };
    
} 



#endif