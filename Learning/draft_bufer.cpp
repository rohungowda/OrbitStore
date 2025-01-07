#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iomanip>
#include <fstream>

// Exception occurred: connect: Connection refused

using boost::asio::ip::tcp;
using std::cout;
using std::endl;

const uint8_t header_bytes = 4;

class SocketConnection{
    public:
        SocketConnection(boost::asio::io_context& io_context)
        : socket_(io_context)
        {}
    
    // return the actual socket not a copy of it
    tcp::socket& getSocket(){
        return socket_;
    }
    
    

    // can write data structs, makles life a lot easier
    void initialize_message(){
       
    }
    void data_read(std::vector<unsigned char> data,const boost::system::error_code& error){

        if(!error){
            initialize_message();

        }else if(error == boost::asio::error::eof){
            cout << "All Data read" << endl;
        }else{
            throw boost::system::system_error(error);
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
        std::shared_ptr<SocketConnection> socketConnection = std::make_shared<SocketConnection>(io_context_);
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
