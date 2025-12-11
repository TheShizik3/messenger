#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

string read_(tcp::socket &socket) {
       boost::asio::streambuf buf;
       boost::asio::read_until(socket, buf, "\0");
       string data = boost::asio::buffer_cast<const char*>(buf.data());
       return data;
}

void write(tcp::socket &sock, std::string text) {
    boost::system::error_code err;
    boost::asio::write(sock, boost::asio::buffer(text), err);
}

int main() {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234));
    tcp::socket sock(io_service);
    acceptor_.accept(sock);
    unsigned long long n = 0;
    while (true) {
    try {
        n++;
        if (n == 20000000000) {
            write(sock, "nyaaah\0"); 
            n = 0;
        }
        // string message = read_(sock);
        // cout << message << endl;
    } catch (std::exception err) {}
    }
    return 0;
} 
bool ServerThread::atWork()() {
    boost::system::error_code err;
    socket->available(err);
    if (err) {
        return false;
    }
    char dump;
    size_t read = socket->read_some(boost::asio::buffer(&dump, 0), err);    
    return !err;
}