#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>


boost::asio::io_context io;

// strand ensures that the argumetns are passed to an Executor in sequential order but the Executor is the one that executes the functions
class TestPrinter{
    public:
        TestPrinter(boost::asio::io_context& io)
        :   strand_(boost::asio::make_strand(io)),
            timer1_(io, boost::asio::chrono::seconds(1)),
            timer2_(io, boost::asio::chrono::seconds(1)),
            count_(0) 
        {
            // binds function to specific strand, automatically sends to strand
            // because print1 is a member function of TestPrinter also expects an isntance, namely the this instance
            timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&TestPrinter::print1, this)));

            // binds function to specific strand, automatically sends to strand
            timer2_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&TestPrinter::print2, this)));
        }

        ~TestPrinter()
        {
            std::cout << "Final count is " << count_ << std::endl;
        }

        void print1()
        {
            if (count_ < 10)
            {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

            timer1_.async_wait(boost::asio::bind_executor(strand_,
                    boost::bind(&TestPrinter::print1, this)));
            }
        }

    void print2()
        {
            if (count_ < 10)
            {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

            timer2_.async_wait(boost::asio::bind_executor(strand_,
                    boost::bind(&TestPrinter::print2, this)));
            }
        }

    private:
        boost::asio::strand<boost::asio::io_context::executor_type> strand_;
        boost::asio::steady_timer timer1_;
        boost::asio::steady_timer timer2_;
        int count_;
};

class printer{
    public:
        printer(boost::asio::io_context& io)
            : timer_(io,boost::asio::chrono::seconds(1)),
              count_(0)
        {
            timer_.async_wait(boost::bind(&printer::print, this)); // include print function in event loop and this refers to the instance of the class
            // this automatically is converted to print(error) and includes all the functiions used in the class that refernce this
        }
    void print()
    {
        if (count_ < 5)
        {
            std::cout << count_ << std::endl;
            ++(count_);
            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            // bind arguments to a function or function object, creates a new function by pre-specifying arguments
            // needs three functions, but the async only expects a function with one error in front, the bind essentially creates a ccallable so that the 
            // first arguemtn is from the boost function and the other two from mt function
            timer_.async_wait(boost::bind(&printer::print, this));
        }
    }
    ~printer()
    {
        std::cout << "Final count is " << count_ << std::endl;
    }
    private:
        boost::asio::steady_timer timer_;
        int count_;

};

// compeletion token (function) to run after event happens
// a callback fn is a function that is passed as an argument to another function
// callback requers a boost system error code as an argument
// but we don't techncially need the status
void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

void print_second(const boost::system::error_code& /*e*/,
    boost::asio::steady_timer* t, int* count)
{
     if (*count < 5)
    {
        std::cout << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        // bind arguments to a function or function object, creates a new function by pre-specifying arguments
        // needs three functions, but the async only expects a function with one error in front, the bind essentially creates a ccallable so that the 
        // first arguemtn is from the boost function and the other two from mt function
        t->async_wait(boost::bind(print_second, boost::asio::placeholders::error, t, count));
    }
}

void synchronous_timer(){
    //boost::asio::io_context io; // execution context for for working with OS and event manager for messages
    boost::asio::steady_timer timer(io,boost::asio::chrono::seconds(2));
    timer.wait(); // blocking wait program stops until timer expires 5 seconds from the timer's creation timestamp

    std::cout << "Hello, world!" << std::endl;
}

void asynchronous_timer(){
    //boost::asio::io_context io;
    boost::asio::steady_timer timer(io,boost::asio::chrono::seconds(5));
    timer.async_wait(&print); // passes the function by refernce
    io.run();
}

void count_async(){
    int count = 0;
    boost::asio::steady_timer timer(io,boost::asio::chrono::seconds(1));
    timer.async_wait(boost::bind(print_second,boost::asio::placeholders::error, &timer, &count));
    io.run();
}

void runprinterClass(){
    printer p(io);
    io.run();
}

void runTestPrinterClass(){
    TestPrinter p(io); // loads both events into the strand so now we are just waiuting for io.run to run()
    std::thread t([&]{ io.run(); }); // & capture all variables in global scope and use those variables in this thread, essentially allows us to run main things in another thread
    // essentially this thread runs the functions in order, not exactly sequentially but allows for thread safe use of resources
    //int functions = io.run();
    //synchronous_timer();
    io.run();
    t.join();
}

int main()
{
    //synchronous_timer();
    //asynchronous_timer();
    //count_async();
    //runprinterClass();
    runTestPrinterClass();
    return 0;
}