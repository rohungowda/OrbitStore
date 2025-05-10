#include "../include/LinkedList.h"

#include <iostream>
#include <cassert>

using namespace Orbit;

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
                // bacuse of monolithic lock don't need to worry about - just to see how many bytes of space we are losing
                counter += ptr->dataSize;

                // this means the ptr is the last node in the list, move last to the previous pointer
                if(ptr->next == nullptr) {last = prev;}

                if(prev == nullptr){
                    // this means we are deleting the root node so we need to move the root to the next, if root is last node
                    // it becomes a nullpointer
                    root = ptr->next;
                }else{
                    // this si middle deltion so regular way
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