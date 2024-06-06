#include <iostream>
#include <fstream>

#include "mkdr.cpp"
#include "crpt.h"
#include "base.h"




using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;
using namespace CryptoPP;

std::string current_user;


void except_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey){
    string filename, owner;
    size_t filename_length, owner_length;

    socket.read_some(boost::asio::buffer(&owner_length, sizeof(size_t)));
    owner.resize(owner_length);
    socket.read_some(boost::asio::buffer(owner.data(), owner_length));

    socket.read_some(boost::asio::buffer(&filename_length, sizeof(size_t)));
    filename.resize(filename_length);
    socket.read_some(boost::asio::buffer(filename.data(), filename_length));

    filename = decryptAES(filename, aesKey);


    // Создаем файл для сохранения получаемых данных.
    ofstream file(get_path_to_dir(owner)/filename, ios::out); //TODO: передавать владельца
    size_t received_bytes = 0;
    cout <<"create file"<<endl;
    size_t file_size;

    // Получаем длину имени файла.
    socket.read_some(boost::asio::buffer(& file_size, sizeof(std::streampos)));

    do {
        // Получаем файл по частям.
        vector<char> buffer(1024);
        size_t bytes_received = socket.read_some(boost::asio::buffer(buffer));
        cout <<"start writing"<<endl;
        std::string ciphertext(buffer.begin(), buffer.end());
        std::string plaintext = decryptAES(ciphertext, aesKey);
        file.write(plaintext.c_str(), plaintext.size());
        received_bytes += bytes_received;
        cout <<  bytes_received<<"Received bytes: " << received_bytes <<endl;

    } while (received_bytes < file_size);
    fetch_editing(owner, filename);
    cout << "Файл был успешно получен: " << filename << endl;
}

void send_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey, const string& owner, const string& user, const string& filename){
    start_editing(owner, filename);
    // Отправляем файл серверу по частям.
    ifstream file;
    file.open("../user_files/"+owner+"/"+filename, std::ifstream::in);
    //cout<<file.good()<<" "<<file.is_open()<<" "<<file.eof() <<endl;
    if(!file.is_open()) throw runtime_error("File net");
    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    write(socket, buffer(&file_size, sizeof(std::streampos)));
    //cout<<sizeof(std::streampos)<<endl;
    file.seekg(0, std::ios::beg);

    while (!file.eof()) {
        vector<char> buffer(1008);
        file.read(buffer.data(), buffer.size());
        cout <<"read sours file "<<endl;
        std::string plaintext(buffer.begin(), buffer.end());
        std::string chipertext = encryptAES(plaintext, aesKey);
        size_t bytes_sent = write(socket, boost::asio::buffer(chipertext));
        cout << "send "<< bytes_sent<<endl;
    }
    file.close();
    cout << "Файл был успешно отправлен: " << filename << endl;
}

void send(tcp::socket& socket, CryptoPP::SecByteBlock aesKey){
    string owner;
    size_t user_length;

    socket.read_some(buffer(&user_length, sizeof(size_t)));
    owner.resize(user_length);
    socket.read_some(buffer(owner.data(), user_length));

    string filename;
    size_t filename_length;

    socket.read_some(buffer(&filename_length, sizeof(size_t)));
    filename.resize(filename_length);
    socket.read_some(buffer(filename.data(), filename_length));

    if (check_editing(owner, filename)) {
        if (current_user == owner) {

            send_file(socket, aesKey, owner,current_user, filename);

        } else {
            if (check_user_access(owner, filename, current_user)) {
                send_file(socket, aesKey, owner,current_user, filename);

            } else {
                throw std::runtime_error("not user's file");
            }
        }
    } else{
        throw "editing now";
    };
}

void authentication(tcp::socket& socket, CryptoPP::SecByteBlock aesKey){
    std::string login, password;
    std::size_t login_length, password_length;
    socket.read_some( boost::asio::buffer(&login_length, sizeof(std::size_t)));
    login.resize(login_length);
    socket.read_some( boost::asio::buffer(login, login_length));

    socket.read_some( boost::asio::buffer(&password_length, sizeof(std::size_t)));
    password.resize(password_length);
    socket.read_some( boost::asio::buffer(password, password_length));
    password = decryptAES(password, aesKey);
    cout << password<<endl;
    // Проверяем логин и пароль.
    bool is_authenticated = check_password_correct(login, hashPass(password));

    if(is_authenticated) current_user = login;

    boost::asio::write(socket, boost::asio::buffer(&is_authenticated, sizeof(bool)));
}

void registration(tcp::socket& socket, CryptoPP::SecByteBlock aesKey){
    std::string login, password;
    std::size_t login_length, password_length;
    socket.read_some( boost::asio::buffer(&login_length, sizeof(std::size_t)));
    login.resize(login_length);
    socket.read_some( boost::asio::buffer(login, login_length));

    socket.read_some( boost::asio::buffer(&password_length, sizeof(std::size_t)));
    password.resize(password_length);
    socket.read_some( boost::asio::buffer(password, password_length));
    // Проверяем логин и пароль.
    try{
        // Отправляем ответ клиенту.
        add_user(login, hashPass(password));
        current_user = login;
        bool response = true;
        boost::asio::write(socket, boost::asio::buffer(&response, sizeof(bool)));
    }catch(...){
        bool response = false;
        boost::asio::write(socket, boost::asio::buffer(&response, sizeof(bool)));
    }
}

void add_access(tcp::socket& socket, CryptoPP::SecByteBlock aesKey ){
    string owner = current_user, filename, user;
    size_t filename_length, user_length;

    socket.read_some( boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    filename.resize(filename_length);
    socket.read_some( boost::asio::buffer(filename, filename_length));

    socket.read_some( boost::asio::buffer(&user_length, sizeof(std::size_t)));
    user.resize(user_length);
    socket.read_some( boost::asio::buffer(user, user_length));

    add_user_access(owner, filename, user);
    cout<< "added"<<endl;
}
void log_out(){
    cout <<"Logout"<<endl;
    current_user.clear();
}
void del_user(tcp::socket& socket, CryptoPP::SecByteBlock aesKey){
    size_t  login_length;
    string login;
    socket.read_some( boost::asio::buffer(&login_length, sizeof(std::size_t)));
    login.resize(login_length);
    socket.read_some( boost::asio::buffer(login, login_length));
    delete_user(login);
}
void del_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey){
    size_t  owner_length;
    string owner;
    socket.read_some( boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    owner.resize(owner_length);
    socket.read_some( boost::asio::buffer(owner, owner_length));

    size_t  filename_length;
    string filename;
    socket.read_some( boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    filename.resize(filename_length);
    socket.read_some( boost::asio::buffer(filename, filename_length));

    delete_file(owner, filename);
}
void session( CryptoPP::SecByteBlock aesKey, tcp::socket& socket ){

    int type;
    socket.read_some(boost::asio::buffer(&type, sizeof(int)));

    if (type == 1) {
        registration(socket, aesKey);
    } else if (type == 2) {
        authentication(socket, aesKey);
    } else if (type == 3) {
        send(socket, aesKey);
    } else if (type == 4) {
        except_file(socket, aesKey);
    } else if (type == 5) {
        add_access(socket, aesKey);
    } else if (type == 6) {
        log_out();
    } else if (type == 7){
        del_user(socket, aesKey);
    } else if (type == 8){
        del_file(socket, aesKey);
    }
}

int main() {

    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        DH dh;
        AutoSeededRandomPool rnd;
        dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, 512);
        SecByteBlock privKey, pubKey, otherPubKey;
        GenerateDHKeys(socket, dh, rnd, privKey, pubKey);
        otherPubKey = receive_crypto_block(socket);
        send_crypto_block(socket, pubKey);
        SecByteBlock sharedKey(dh.AgreedValueLength());
        if(!dh.Agree(sharedKey, privKey, otherPubKey))
            throw std::runtime_error("Failed to reach shared secret");

        std::cout <<"suc generated key"<<endl;
        CryptoPP::SecByteBlock aesKey = deriveAESKeyFromDH(sharedKey);


        current_user = "Nastya";
        while (true) {

            try{
                session(aesKey, socket);
            } catch(...){
                continue;
            }
            cout <<"end command"<< current_user<<endl;
        }
        socket.close();
    }
    catch (exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}