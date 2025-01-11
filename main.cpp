#include "globals.h"
#include "Scheduler/Scheduler.h"

#include <iostream>
//#include "Schedulers/IOScheduler.h"

bool run = true;


void test(int task){
      std::this_thread::sleep_for(std::chrono::seconds(2));
      std::cout << task << " ID Completed" << std::endl;
}

int main(){
    LogicalPointer<int> IOpointer(0);
    // Scheduler<int,BasicComparator<int>> IOManager = Scheduler<int, BasicComparator<int>>(IOpointer,BasicComparator<int>(IOpointer),test); Regular Queue
    Scheduler<int,Comparator<int>> IOManager = Scheduler<int, Comparator<int>>(IOpointer,Comparator<int>(IOpointer),test);
    
  std::cout << "created IO Manager" << std::endl;
    std::thread temp([&IOManager](){
            IOManager.Push(5);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            IOManager.Push(6);
            std::this_thread::sleep_for(std::chrono::seconds(3));
            IOManager.Push(3);
    });
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::thread temp1([&IOManager](){
            IOManager.Push(1);
            IOManager.Push(3);
            std::this_thread::sleep_for(std::chrono::seconds(9));
            IOManager.Push(4);
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread temp2([&IOManager](){
            IOManager.Push(10);
            IOManager.Push(15);
            IOManager.Push(3);
    });

    temp.join();
    temp1.join();
    temp2.join();


    return 0;
}