#include <iostream>
#include <thread>

bool run = true;




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

        }

        void shutdown(){
            std::cout << "Shutting down DiskAccess..." << std::endl;
        }

    
    private:
        const static int PageSize = 4096;
        std::thread database_thread;

};




/*
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

*/

/*
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
            consumer_thread = std::thread(&Scheduler::ConsumeMessages, this);
        }

        ~Scheduler(){
            consumer_thread.join();
        }
    
        void Push(T data){
            std::unique_lock<std::mutex> lock(mtx);
            pq.push(data);
            lock.unlock();

            cv.notify_all();

        }

        void ConsumeMessages(){
            while(run){
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock,[this]() { return !pq.empty();});
                

                T task = pq.top();
                pq.pop();
                logical_pointer = task;
                lock.unlock();
                //std::cout << "Logical Pointer Currently at " << logical_pointer << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::cout << task << " ID Completed" << std::endl;

            }
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
        std::thread consumer_thread;
        std::mutex mtx;
        std::condition_variable cv;
};
*/


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