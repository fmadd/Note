#include <iostream>
#include <string>
#include "utils.hpp"


using boost::asio::ip::tcp;
using namespace std;
using namespace CryptoPP;
using namespace boost::asio;

//void auth(tcp::socket& socket, CryptoPP::SecByteBlock aesKey ){
//    bool response = false;
//    while(!response){
//        std::cout << "Введите 1 что бы зарегестрироваться, 2 что бы авторизоваться" << std::endl;
//        int flag;
//        std::cin >> flag;
//        if(flag == 1) response = registration(socket, aesKey);
//        else if (flag == 2)   response = authentication(socket, aesKey);
//        if (response == 0) std::cout << "Неккоректный логин или пароль\n";
//    }
//}
//
//int session(tcp::socket& socket, CryptoPP::SecByteBlock aesKey){
//    while(true){
//        std::cout << "Введите "
//                     " 3 что бы скачать файл \n"
//                     " 4 что бы загрузить файл \n"
//                     " 5 что бы дать права на редактирование \n"
//                     " 6 что бы выйти из аккаунта\n"
//                     " 7 что бы удалить аккаунт\n"
//
//                  << std::endl;
//        int flag;
//
//        std::cin >> flag;
//
//        switch(flag){
//            case 3:
//                try {
//                    except_file(socket, aesKey);
//                } catch(...){
//                    cout << "Файл недоступен\n";
//                }
//                break;
//            case 4:
//                try {
//                    send_file(socket, aesKey);
//                } catch(...) {
//                    cout << "Файла не существует\n";
//                }
//                break;
//            case 5:
//                add_access(socket,aesKey);
//                break;
//            case 6:
//                log_out(socket);
//                return 0;
//            case 7:
//                delete_user(socket);
//                return 0;
////            case 8:
////                delete_file(socket,aesKey);
////                break;
//        }
//    }
//}
//int main(int, char**) {
//    try {
//
//
//        boost::asio::io_service io_service;
//        tcp::socket socket(io_service);
//        socket.connect(tcp::endpoint(tcp::v4(), 1234));
//
//        CryptoPP::SecByteBlock aesKey = get_aesKey(socket);
//        while(true) {
//
//            auth(socket, aesKey);
//            session(socket, aesKey);
//
//        }
//    }
//    catch (exception& e) {
//        cerr << "Mistake: " << e.what() << endl;
//    }
//
//    return 0;
//}
