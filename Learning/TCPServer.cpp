#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string(){
    using namespace std; // uses the namespace in the scope of the function
    // holds the number of seconds
    time_t now = time(0);

    return ctime(&now); // ctime takes a pointer to UNIX number of seconds
}

// shared_ptr protects the tcp_connection ibject from being destroyed
// basically waht enable shared does is it tells the tcp_connection that its being used by somebody and should still be there
// we are essebtially telling tcp_connection how to make a shared pointer to itself, esssentially to keep it alive
// whenobject calles shared_from_this it is essentially owned by a shared_ptr somewhere
// we can't use this bc it might be destoryed, so instead the shared pointer allows us to create a pointer which points to the obejct to keep it alive until nobody is using it
// by calling shared_this, we are sayignt hat this process is no being owned by an async completion handler
class tcp_connection
  : public std::enable_shared_from_this<tcp_connection> 
{
    public:
        typedef std::shared_ptr<tcp_connection> pointer; // creates a pointer alias for shared_ptr<tcp_connection>

        static pointer create(boost::asio::io_context& io_context)
        {
            return pointer(new tcp_connection(io_context));
        }

        tcp::socket& socket()
        {
            return socket_;
        }

        void start()
        {
            message_ = make_daytime_string(); // need to keep the data valid until async process is done
            // taskes in the socket the message in the buffer and the binded function handle_write
            boost::asio::async_write(socket_, boost::asio::buffer(message_), boost::bind(&tcp_connection::handle_write, shared_from_this()));
        }

    private:
        tcp::socket socket_;
        std::string message_;
        tcp_connection(boost::asio::io_context& io_context)
        : socket_(io_context)
        {
        }
        void handle_write(){} // just needed to ensure the async runs and sends the data, can use it do something on completion
};

class tcp_server{
    public:
        tcp_server(boost::asio::io_context& io_context, int port)
        : io_context_(io_context),
          acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
        {
            start_accept();
        }

        void start_accept(){
            //creates a socket and waits for a new connection
             tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

            // takes in a new connection's socket and binds the handler function with this isntance, the connection variable, and an error placeholder
            acceptor_.async_accept(new_connection->socket(),boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error));
        }
            // once it gets a message it handles it and waits for a new one
          void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
            {
                if (!error)
                {
                new_connection->start();
                }

                start_accept();
            }

    private:
        boost::asio::io_context& io_context_; // cannot copy one into another isntance, need to pass by reference
        tcp::acceptor acceptor_;

};

void startAsyncServer(int port){
    try{
         boost::asio::io_context io_context;
         tcp_server server(io_context, port);
         io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void startServer(int port){

    try
    {
        boost::asio::io_context io_context;
        // accepts connections on port 13 for IPV4 connections
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

         for (;;)
        {

            tcp::socket socket(io_context);
            // handles one connection at a time
            acceptor.accept(socket); // blockks thread until connection is accepted

            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch(const std::exception& e){
        std::cout << "Exception occurred: " << e.what() << std::endl;
    }
}

int main(){
    std::cout <<  "Starting Server" << std::endl;
    //startServer(8080);
    startAsyncServer(8080);
}