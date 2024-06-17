#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include <string>
#include <fstream>

#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/dh2.h>
#include <cryptopp/dh.h>
#include <cryptopp/secblock.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>

#include "crpt.h"

using boost::asio::ip::tcp;
using namespace std;
using namespace CryptoPP;
using namespace boost::asio;


string current_user;



bool registration(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& login, const std::string& password_plain) {
    int number = 1;

    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::string password = encryptAES(password_plain, aesKey);

    std::size_t login_length = login.size();
    std::size_t password_length = password.size();

    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
    boost::asio::write(socket, boost::asio::buffer(&password_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(password, password_length));
    bool response;
    socket.read_some(boost::asio::buffer(&response, sizeof(bool)));
    if(response) current_user = login;
    return response;
}
bool authentication(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& login, const std::string& password_plain) {
    int number = 2;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::string password = encryptAES(password_plain, aesKey);

    std::size_t login_length = login.size();
    std::size_t password_length = password.size();

    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
    boost::asio::write(socket, boost::asio::buffer(&password_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(password, password_length));

    bool response;
    socket.read_some(boost::asio::buffer(&response, sizeof(bool)));
    if(response) current_user = login;
    return response;
}

void except_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey, string owner, string filename) {

    int number = 3;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::size_t owner_length = owner.size();
    std::size_t filename_length = filename.size();

    boost::asio::write(socket, boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(owner, owner_length));
    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename, filename_length));



    size_t received_bytes = 0;
    size_t file_size;
    socket.read_some(boost::asio::buffer(&file_size, sizeof(std::size_t)));

    if(file_size==0){
        throw std::runtime_error("Файл недоступен или пуст");
    }

    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path parent_path = current_path.parent_path();
    std::filesystem::path user_path = parent_path / "userfiles";
    std::filesystem::create_directory(user_path);
    ofstream file(user_path / filename, ios::out);

    do {
        vector<char> buffer(1024);
        size_t bytes_received = socket.read_some(boost::asio::buffer(buffer));
        //cout << bytes_received<<endl;
        std::string ciphertext(buffer.begin(), buffer.end());
        std::string plaintext = decryptAES(ciphertext, aesKey);

        if(received_bytes + bytes_received - file_size > 0){
            plaintext.resize(file_size - received_bytes);
        }
        file.write(plaintext.c_str(), plaintext.size());
        received_bytes += bytes_received;

    } while (received_bytes < file_size);
}



void send_file(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& owner, const std::string& filename) {
    int number = 4;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    ifstream file;
    file.open("../userfiles/" + filename, std::ifstream::in);
    if (!file.is_open()) throw std::runtime_error("File not exist");

    std::size_t owner_length = owner.size();
    boost::asio::write(socket, boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(owner, owner_length));

    std::string filename_enc = encryptAES(filename, aesKey);

    std::size_t filename_enc_length = filename_enc.size();
    boost::asio::write(socket, boost::asio::buffer(&filename_enc_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename_enc));



    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    boost::asio::write(socket, boost::asio::buffer(&file_size, sizeof(size_t)));

    file.seekg(0, std::ios::beg);

    while (!file.eof()) {
        vector<char> buffer(1000);
        file.read(buffer.data(), buffer.size());
        std::string plaintext(buffer.begin(), buffer.end());
        std::string ciphertext = encryptAES(plaintext, aesKey);
        std::string pl = decryptAES(ciphertext, aesKey);
        size_t bytes_sent = socket.write_some(boost::asio::buffer(ciphertext));

        //cout <<bytes_sent<<" process\n";
    }
    file.close();
}


void add_access(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& filename, const std::string& user) {
    int number = 5;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::size_t filename_length = filename.size();
    std::size_t user_length = user.size();

    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename, filename_length));
    boost::asio::write(socket, boost::asio::buffer(&user_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(user, user_length));
}

void log_out(tcp::socket& socket) {
    int number = 6;
    current_user.resize(0);
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

}


void delete_user(tcp::socket& socket) {
    int number = 7;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::string login = current_user;
    std::cout << current_user << " удален\n";

    std::size_t login_length = login.size();
    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
}

void delete_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey, const std::string& owner, const std::string& filename) {
    int number = 8;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::size_t owner_length = owner.size();
    std::size_t filename_length = filename.size();

    boost::asio::write(socket, boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(owner, owner_length));
    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename, filename_length));
}

