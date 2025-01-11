#ifndef COMPARATORS_H
#define COMPARATORS_H 

/*
act like a regular queue
    bool operator()(const int& a, const int& b) const {
        return false; // Always return false
    }
*/

#include "LogicPointer.h"

template<typename T>
class Comparator{
    public:
        Comparator(LogicalPointer<T> &shared)
        : shared_instance(shared) {}

        bool operator()(T& a, T& b){
            T pointer = shared_instance.getLogicalPointer();

            if ((a < pointer && b < pointer) || (a >= pointer && b >= pointer)){
                return a > b; // if true swap, else keep
            }else{
                return a < b;
            }
        }

    private:
        const LogicalPointer<T> &shared_instance;
};

template<typename T>
class BasicComparator{
    public:
        BasicComparator(LogicalPointer<T> &shared)
        : shared_instance(shared) {}

        bool operator()(T& a, T& b){
            return false;
        }

    private:
        const LogicalPointer<T> &shared_instance;
};

#endif