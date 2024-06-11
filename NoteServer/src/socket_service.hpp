
#pragma once
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;
using namespace CryptoPP;

tcp::socket init_socket(){
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
    tcp::socket socket(io_service);
    acceptor.accept(socket);
    return socket;
}

class socket_service {
private:
    tcp::socket _socket;
public:
    socket_service() : _socket(init_socket()) {};

    size_t writeInSocket(const_buffers_1 bf) {
        return write(_socket, bf);
    };

    size_t writeInSocket(boost::asio::mutable_buffers_1 bf) {
        return write(_socket, bf);
    };

    size_t read_filesize() {
        size_t file_size;
        _socket.read_some(boost::asio::buffer(&file_size, sizeof(size_t)));
        return file_size;
    }

    size_t read_file_buffer(std::vector<char> &buffer) {
        return _socket.read_some(boost::asio::buffer(buffer));
    }

    size_t read_buffer(boost::asio::mutable_buffers_1 buffer) {
        return _socket.read_some(boost::asio::buffer(buffer));
    }

    size_t read_request() {
        int type;
        _socket.read_some(boost::asio::buffer(&type, sizeof(int)));
        return type;
    }

    std::string get_string() {
        std::string text;
        size_t length = read_filesize();
        text.resize(length);
        _socket.read_some(buffer(text, length));
        return text;
    }

    void close() {
        _socket.close();
    }
};