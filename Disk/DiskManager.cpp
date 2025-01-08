#include <iostream>
#include <queue>
#include <thread>



class DiskManager{
    public:
    static int logical_pointer;
        DiskManager(){}
        
        // coroutine
        void ProcessRequest(int pageID){
            pq.push(pageID);
        }

        // coroutine
        void ProcessQueue(){
            while(!pq.empty()){
                std::cout << pq.top() << " ";
                pq.pop();
            }
        }

       
    private:
        std::priority_queue<int, std::vector<int>, Comparator> pq;
};

class Comparator{
    public:
        bool operator()(int a, int b) const{
            int pointer = DiskManager::logical_pointer;

            if ((a < pointer && b < pointer) || (a >= pointer && b >= pointer)){
                return a > b; // if true swap, else keep
            }else{
                return a < b;
            }
        }
};

int DiskManager::logical_pointer = 0;

int main(){


    int test[10] = {0,1,4,5,6,7,8,10,9,14};

   
    std::cout << std::endl;

    return 0;
}