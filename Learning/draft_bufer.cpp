#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

// Exception occurred: connect: Connection refused

using boost::asio::ip::tcp;
using std::cout;
using std::endl;

class SocketConnection{
    public:
        SocketConnection(boost::asio::io_context& io_context)
        : socket_(io_context)
        {}
    
    // return the actual socket not a copy of it
    tcp::socket& getSocket(){
        return socket_;
    }
    
    void initialize_message(){
        // right now do it with async read with fixes amount of bytes protocol - shared of 512 bytes or something
        std::vector<char> data(15);
        std::shared_ptr<boost::asio::mutable_buffer> buffer2 = std::make_shared<boost::asio::mutable_buffer>(boost::asio::buffer(data));
        //boost::asio::mutable_buffer buffer = boost::asio::buffer(data);
        boost::asio::read(socket_,*buffer2); // buffer.data points to the address of the vector
        cout << *(static_cast<char *>(buffer2->data())) << endl;

        for(char d : data){
            cout << d << endl;
        }
    }

    private:
        tcp::socket socket_;
};

class Server{
    public:
        Server(boost::asio::io_context& io_context, int port)
        :   io_context_(io_context),
            acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
        {
            start_accept();
        }
        void start_accept(){
        //creates a socket and assigns a shared pointer to the socket
            std::shared_ptr<SocketConnection> socketConnection = std::make_shared<SocketConnection>(io_context_);

        // takes in a new connection's socket and binds the handler function with this isntance, the connection variable, and an error placeholder
        // because I am passign this, I need to have a pointer to that function, so it knows how to call it, if it was a function outside, could just have regular function
        acceptor_.async_accept((*socketConnection).getSocket(), boost::bind(&Server::handle_accept, this, socketConnection, boost::asio::placeholders::error));
        }

        void handle_accept(std::shared_ptr<SocketConnection> socketConnection, const boost::system::error_code& error)
        {
            if (!error)
            {
                socketConnection->initialize_message();
            }

            start_accept();
        }
    private:
        boost::asio::io_context& io_context_;
        tcp::acceptor acceptor_;
};



int main(){

    boost::asio::io_context io_context;

    cout << "Starting Server" << endl;
    Server server(io_context, 8080);
    io_context.run();

    return 0;
}