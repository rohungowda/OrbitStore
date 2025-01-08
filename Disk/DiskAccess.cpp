#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>




template<typename T>
class Scheduler{
    public:

    class Comparator{
        public:
            Comparator(Scheduler<T> &shared)
            : shared_instance(shared)
            {}

            bool operator()(T a, T b) const{
                int pointer = shared_instance.getLogicalPointer();

                if ((a < pointer && b < pointer) || (a >= pointer && b >= pointer)){
                    return a > b; // if true swap, else keep
                }else{
                    return a < b;
                }
            }
        private:
            const Scheduler<T> &shared_instance;
    };

        Scheduler()
        : logical_pointer(0), pq(Scheduler<T>::Comparator(*this))
        {
            //scheduler_thread = std::thread(&Scheduler::Monitor, this);
        }
    
        void Push(T data){
            std::unique_lock<std::mutex> lck(mtx);
            pq.push(data);
            if(!(ready.load())){
                ready = true;
                cv.notify_all();
            }

        }

        T Pop(){
            std::unique_lock<std::mutex> lck(mtx);
            T task = pq.top();
            pq.pop();
            logical_pointer = task;
            return task;
        }

        T getLogicalPointer()const{
            return logical_pointer;
        }

        void load(){
            std::cout <<pq.size() <<std::endl;
        }
    
    private:
        std::priority_queue<T, std::vector<T>, Scheduler<T>::Comparator> pq;
        T logical_pointer;
        std::thread scheduler_thread;
        std::mutex mtx;
};


class DiskAccess{
    public:
        DiskAccess()
        {
        }

        void init(){
            database_thread = std::thread(&DiskAccess::handleRequests, this);
        }

        void read(int pageID, char* buffer){
            int byte_pointer = pageID * PageSize;
            std::cout << "Reading pageID " << byte_pointer << " into buffer address " << buffer << std::endl;
        }


        void write(int pageID, char* buffer){
            int byte_pointer = pageID * PageSize;
            std::cout << "Writing to pageID " << byte_pointer << " with data from buffer address " << buffer << std::endl; 
        }

        ~DiskAccess(){
            database_thread.join();
        }

        void handleRequests(){
            std::cout << "Thread Started" << std::endl;
            std::unique_lock<std::mutex> lck(mtx);

            while(!(flag.load())){
                while(!(ready.load())) cv.wait(lck);

                if(flag.load()){
                    shutdown();
                    break;
                }

                std::cout << "Action happens" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                ready = false;
                cv.notify_all();
            }


        }

        void shutdown(){
            std::cout << "Shutting down DiskAccess..." << std::endl;
        }

    
    private:
        const static int PageSize = 4096;
        std::thread database_thread;

};

int main(){

    Scheduler<int> IOManager = Scheduler<int>(); // think about std::shared, does same thng like new but automatic memory management
    std::cout << "created IO Manager" << std::endl;
    IOManager.Push(5);
    IOManager.Push(6);
    IOManager.Push(7);
    IOManager.Push(1);
    IOManager.Push(2);
    std::cout << "Simualte time" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Time done" << std::endl;
    IOManager.Push(7);
    std::cout << "Begins Adding" << std::endl;
    IOManager.Push(8);
    //IOManager.Push(9);

    return 0;
}

/*

    std::cout << "Starts Disk Access" << std::endl;
    DiskAccess manager = DiskAccess();
    manager.init();

    std::unique_lock<std::mutex> lck(mtx);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Notification" << std::endl;
    ready = true;
    cv.notify_all();
    while((ready.load())) cv.wait(lck);
    std::cout << "Confirmation Recieved" << std::endl;
    flag = true;
    ready = true;
    cv.notify_all();



    IOManager.Push(5);
    IOManager.Push(6);
    IOManager.Push(7);
    IOManager.Push(1);
    IOManager.Push(2);
    std::cout << "Simualte time" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Time done" << std::endl;
    IOManager.Push(7);
    std::cout << "Begins Adding" << std::endl;
    IOManager.Push(8);
    IOManager.Push(9);
    //flag = true;


*/