#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

// Exception occurred: connect: Connection refused

using boost::asio::ip::tcp;
using std::cout;
using std::endl;

boost::asio::io_context io_context;

    struct Header{
        uint8_t type;
        uint8_t chunkID;
        uint8_t totalChunks;
        uint16_t pageID;
        uint16_t totalBytes;

        Header(uint8_t type_, uint8_t chunkID_, uint16_t pageID_, uint8_t totalChunks_, uint8_t totalBytes_) 
        : type(type_), chunkID(chunkID_), pageID(pageID_), totalChunks(totalChunks_), totalBytes(totalBytes_) {}


    };

// argv is a pointer to a bunvh of pointers(arrays)
void startup_client(std::string host){
 try
    {


        // resolver takes a server name and the name of the service
        // reuturns a lit of endpoints of type 
        tcp::resolver resolver(io_context);

        // can iterate through with begin(), end()
        // ? tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
        // tcp::resolver::iterator iterator = resolver.resolve(query);
        tcp::resolver::results_type endpoints = resolver.resolve(host, "8080");
    
        // creates a tcp sopcker
        tcp::socket socket(io_context);
        // automatically connects to endpoints with IPv4 and IPv6 endpoints
        boost::asio::connect(socket,endpoints);

        std::vector<Header> buffer = {Header(1,2,3,4,5), Header(5,4,3,2,1)};


        cout << "Sending Data" << endl;
        boost::asio::write(socket,boost::asio::buffer(buffer));
        cout << "Finished Writing" << endl;

    } 
    catch(const std::exception& e){
        std::cout << "Exception occurred: " << e.what() << std::endl;
    }


}

class Buffers
  : public std::enable_shared_from_this<Buffers>
{
    public:
        Buffers(size_t size)
        : data_(new char [size]),
          buffer(data_.get(), size) // includes a pointer to data and a size
        {}
    boost::asio::mutable_buffer get_buffer(){
        return buffer;
    }

    private:
        std::shared_ptr<char[]> data_;
        boost::asio::mutable_buffer buffer;
};
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

    // empty contructor
    boost::asio::mutable_buffer header = boost::asio::mutable_buffer();
    
    // spans are essentially pointers but allow not only at indexing and subranging but alsoknowing the size of the data
    // however spans don't resize
    unsigned char data[10] = "Testing"; 

    // pass by reference means pass the actual object not a sopy of the value
    boost::asio::mutable_buffer Span = boost::asio::mutable_buffer(data, sizeof(data));

    cout << Span.size() << endl;
    char* pointer = static_cast<char*>(Span.data());
    cout << pointer << endl;
    cout << *pointer << endl;

    startup_client("172.31.125.133");

    return 0;
}


/*
        for(;;){

            boost::system::error_code error;
            size_t len = socket.read_some(, error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buffer.data(), len);
        }


          cout << "Message Recieved!!!" << endl;
        //std::shared_ptr<boost::asio::buffer> buffer(std::vector<char>(15));

        boost::system::error_code error;

        for(;;){

        //size_t len = socket_.read_some(*buffer, error);
        //cout << len << endl;
        //cout.write(static_cast<const char*>(buffer->data()), len);

        size_t len = boost::asio::read(socket_, *buffer, boost::asio::transfer_exactly(15), error);

        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.


        // need to understand the difference between mutable sequences with array and using a vector for mutable sequences

        }

 
*/