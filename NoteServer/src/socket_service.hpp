
#pragma once
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;
using namespace CryptoPP;

tcp::socket init_socket(){
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);

    return socket;
}
/**
 * \file socket_service.h
 * \brief Класс socket_service
 */

/**
 * \class socket_service
 * \brief Класс, предоставляющий сервисы для работы с сокетом
 */
class socket_service {
private:
    tcp::socket _socket; /**< Сокет для обмена данными */
public:
    /**
     * \brief Конструктор класса socket_service.
     * Инициализирует сокет с помощью функции init_socket().
     */
    socket_service() : _socket(init_socket()) {};
    /**
     * \brief Инициализация socket_service.
     * Инициализирует сокет с помощью функции socket_init().
     */
    void socket_init(){
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
        acceptor.accept(_socket);
        acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    }
    /**
     * \brief Записывает данные в сокет.
     * \param bf Буфер для записи.
     * \return Количество записанных байт.
     */
    virtual size_t writeInSocket(const_buffers_1 bf) {

        return write(_socket, bf);
    };
    /**
     * \brief Записывает данные в сокет.
     * \param bf Буфер для записи.
     * \return Количество записанных байт.
     */
    virtual size_t writeInSocket(boost::asio::mutable_buffers_1 bf) {
        return write(_socket, bf);
    };
    /**
     * \brief Читает размер файла из сокета.
     * \return Размер файла.
     */
    size_t read_filesize() {
        size_t file_size;
        _socket.read_some(boost::asio::buffer(&file_size, sizeof(size_t)));
        return file_size;
    }
    /**
    * \brief Читает данные файла из сокета в буфер.
    * \param buffer Буфер для чтения.
    * \return Количество прочитанных байт.
    */
    size_t read_file_buffer(std::vector<char> &buffer) {
        return _socket.read_some(boost::asio::buffer(buffer));
    }
    /**
     * \brief Читает данные из сокета в буфер.
     * \param buffer Буфер для чтения.
     * \return Количество прочитанных байт.
     */
    size_t read_buffer(boost::asio::mutable_buffers_1 buffer) {
        return _socket.read_some(boost::asio::buffer(buffer));
    }
    /**
    * \brief Читает тип запроса из сокета.
    * \return Тип запроса.
    */
    size_t read_request() {
        int type;
        _socket.read_some(boost::asio::buffer(&type, sizeof(int)));
        return type;
    }
    /**
     * \brief Получает строку из сокета.
     * \return Полученная строка.
     */
    virtual std::string get_string() {
        std::string text;
        size_t length = read_filesize();
        text.resize(length);
        _socket.read_some(buffer(text, length));
        return text;
    }
    /**
     * \brief Закрывает сокет.
     */
    void close() {
        _socket.close();
    }
};