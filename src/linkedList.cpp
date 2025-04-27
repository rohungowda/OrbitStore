#include "../include/LinkedList.h"



using namespace Orbit;


#include <iostream>
#include <cassert>

using namespace std;

// have to be carefull with the char* data because once it goes out of scope, problems happen

LinkedList::LinkedList() : root(nullptr), last(nullptr) {}

void LinkedList::createEndNode(char* ptr, size_t size){
    Node* temp = new Node(ptr,size);

    if(root == nullptr){
        root = temp;
        last = temp;
    }else{
        last->next = temp;
        last = last->next;
    }



}

LinkedList::~LinkedList(){
    Node* ptr = root;
    while(ptr != nullptr){
        Node* save = ptr->next;
        delete (ptr);
        ptr = save;
    }

    root = nullptr;
    last = nullptr;

    assert(root == nullptr);
}

char* LinkedList::findCandidate(size_t size){
    Node* ptr = root;
    Node* prev = nullptr;

    while(ptr != nullptr){
        if(ptr->dataSize >= size){

            // return ptr
            char* returnPtr = ptr->pointer;

            // reflect changes in node values

            ptr->dataSize -= size;
            ptr->pointer += size;

            if(ptr->dataSize <= limit){
                // delete node
                if(ptr->next == nullptr) {last = prev;}

                if(prev == nullptr){
                    root = ptr->next;

                }else{
                    prev->next = ptr->next;
                }

                delete ptr;

            }


            return returnPtr;
        }

        prev = ptr;
        ptr = ptr->next;
    }

    return nullptr;

}