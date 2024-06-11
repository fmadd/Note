
#pragma once

#include <iostream>
#include <fstream>

#include "path_util.hpp"
#include "crypto.hpp"
#include "base.h"
#include "socket_service.hpp"

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;
using namespace CryptoPP;

std::string current_user;


class server_service {
private:
    socket_service &_socket;
    DB_service _db;
    CryptoPP::SecByteBlock _aesKey;

public:
    explicit server_service(socket_service &socket, const DB_service &db, CryptoPP::SecByteBlock aesKey) : _socket(socket),
                                                                                                        _db(db),
                                                                                                        _aesKey(aesKey) {};

    void except_file() {
        string filename, owner;

        owner = _socket.get_string();
        filename = _socket.get_string();
        filename = decryptAES(filename, _aesKey);

        ofstream file(get_path_to_dir(owner) / filename, ios::out);
        size_t received_bytes = 0;
        size_t file_size =  _socket.read_filesize();

        do {
            // Получаем файл по частям.
            vector<char> buffer(1024);
            size_t bytes_received = _socket.read_file_buffer(buffer);
            std::string ciphertext(buffer.begin(), buffer.end());
            std::string plaintext = decryptAES(ciphertext, _aesKey);
            file.write(plaintext.c_str(), plaintext.size());
            received_bytes += bytes_received;

        } while (received_bytes < file_size);
        _db.fetch_editing(owner, filename);
    }

    void send_file(const string &owner, const string &user, const string &filename) {
        _db.start_editing(owner, filename);
        ifstream file;
        file.open("../user_files/" + owner + "/" + filename, std::ifstream::in);
        if (!file.is_open()) throw runtime_error("File not exist");
        file.seekg(0, std::ios::end);
        std::streampos file_size = file.tellg();
        _socket.writeInSocket(buffer(&file_size, sizeof(std::streampos)));
        file.seekg(0, std::ios::beg);

        while (!file.eof()) {
            vector<char> buffer(1008);
            file.read(buffer.data(), buffer.size());
            std::string plaintext(buffer.begin(), buffer.end());
            std::string chipertext = encryptAES(plaintext, _aesKey);
            size_t bytes_sent = _socket.writeInSocket(boost::asio::buffer(chipertext));
        }
        file.close();
    }

    void send() {
        string owner = _socket.get_string();
        string filename = _socket.get_string();

        if (_db.check_editing(owner, filename)) {
            if (current_user == owner) {
                send_file(owner, current_user, filename);
            } else {
                if (_db.check_user_access(owner, filename, current_user)) {
                    send_file(owner, current_user, filename);

                } else {
                    throw std::runtime_error("not user's file");
                }
            }
        } else {
            throw std::runtime_error("file editing right now");
        }
    }

    void authentication() {
        std::string login, password;

        login = _socket.get_string();
        password = decryptAES(_socket.get_string(), _aesKey);
        bool is_authenticated = _db.check_password_correct(login, hashPass(password));
        if (is_authenticated) current_user = login;
        _socket.writeInSocket(boost::asio::buffer(&is_authenticated, sizeof(bool)));
    }

    void registration() {

        std::string login, password;
        login = _socket.get_string();
        password = _socket.get_string();
        bool response;
        try {
            // Отправляем ответ клиенту.
            std::cout << "reg2"<<endl;
            _db.add_user(login, hashPass(password));
            current_user = login;
            response = true;

        } catch (...) {
            response = false;
        }
        _socket.writeInSocket(boost::asio::buffer(&response, sizeof(bool)));
    }

    void add_access() {
        string owner = current_user, filename, user;
        filename = _socket.get_string();
        user = _socket.get_string();
        _db.add_user_access(owner, filename, user);
    }

    void log_out() {
        cout << "Logout" << endl;
        current_user.clear();
    }

    void del_user() {
        string login = _socket.get_string();
        _db.delete_user(login);
    }

    void del_file() {
        string owner = _socket.get_string();
        string filename = _socket.get_string();
        _db.delete_file(owner, filename);
    }

    void session() {

        int type = _socket.read_request();

        if (type == 1) {
            std::cout << "Type"<<std::endl;
            registration();
        } else if (type == 2) {
            authentication();
        } else if (type == 3) {
            send();
        } else if (type == 4) {
            except_file();
        } else if (type == 5) {
            std::cout<<"add"<<std::endl;
            add_access();
        } else if (type == 6) {
            log_out();
        } else if (type == 7) {
            del_user();
        } else if (type == 8) {
            del_file();
        } else{
            _socket.close();
            throw std::runtime_error("wrong request");
        }
    }

};

