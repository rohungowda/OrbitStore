
#ifndef LOGICPOINTER_H
#define LOGICPOINTER_H 

template<typename T>
class LogicalPointer{
    public:
        LogicalPointer(T inital): logical_pointer(inital) {}
        T getLogicalPointer() const{
            return logical_pointer;
        }
        void setLogicalPointer(T pointer){
            logical_pointer = pointer;
        }
    private:
        T logical_pointer;
};

#endif