#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/crpt.h"
#include "../src/base.h"
#include "../src/server.hpp"

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;

using ::testing::_;
using ::testing::Return;

// Мок класс для socket
class MockSocket : public boost::asio::ip::tcp::socket {
public:
    MockSocket(boost::asio::io_service& io_service) : boost::asio::ip::tcp::socket(io_service) {};
//    MOCK_METHOD(std::size_t, read_some, (MutableBufferSequence), (override));
//    MOCK_METHOD(std::size_t, write_some, (boost::asio::const_buffer, std::size_t));

    std::size_t read_some(mutable_buffer myBuffer) {
        cout << "HE\n";
      return 5;
    };

};

TEST(AuthenticationTest, Success) {
    // читаем из сокета логин пас и проверяем его и отсылаем в бд
    boost::asio::io_service io_service;
    MockSocket mockSocket(io_service);

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::SecByteBlock aesKey(CryptoPP::AES::MAX_KEYLENGTH);
    rng.GenerateBlock(aesKey, aesKey.size());

//    ON_CALL(mockSocket, read_some).WillByDefault(Return(5));

    registration(mockSocket, aesKey);
    // проверяем что сокет принял бул

    ASSERT_TRUE(true == false);
}

// Тест для проверки успешной аутентификации
//TEST(AuthenticationTest, Success) {
//    boost::asio::io_service io_service;
//    MockSocket socket(io_service);
//    std::string login = "user";
//    std::string password = "password";
//
//    CryptoPP::AutoSeededRandomPool rng;
//    CryptoPP::SecByteBlock aesKey(CryptoPP::AES::MAX_KEYLENGTH);
//    rng.GenerateBlock(aesKey, aesKey.size());
//    std::string encryptedPassword = encryptAES(password, aesKey);
//    std::size_t passwordLength = encryptedPassword.size();
//
//
//
//    EXPECT_CALL(socket, read_some(_, sizeof(std::size_t))).Times(1).WillOnce(Return(4));
//    EXPECT_CALL(socket, read_some(_, passwordLength)).Times(1).WillOnce(Return(10));
//    EXPECT_CALL(socket, write_some(_, sizeof(bool))).Times(1).WillOnce(Return(2));
//
//    authentication(socket, aesKey);
//
//
//
//    bool is_authenticated = true;
//
////    EXPECT_CALL(socket, write_some(_, sizeof(bool)))
////
////    .WillOnce(testing::DoAll(testing::SetArgPointee<1>(sizeof(bool)), Return(sizeof(bool))));
//
//    // Вызываем функцию authentication с использованием мок-объекта
//    authentication(socket, aesKey);
//    // Выполняем тестируемый код
//    is_authenticated = check_password_correct(login, hashPass(password));
//    ASSERT_TRUE(is_authenticated == false);
//}

//// Тест для проверки неуспешной аутентификации
//TEST(AuthenticationTest, Failure) {
//MockSocket socket;
//std::string login = "hacker";
//std::string password = "wrong_password";
//std::string encryptedPassword = encryptAES(password, aesKey);
//std::size_t passwordLength = encryptedPassword.size();
//
//// Устанавливаем ожидания для вызовов мок-объекта
//EXPECT_CALL(socket, read_some(_, sizeof(std::size_t))).Times(1).WillOnce(Return());
//EXPECT_CALL(socket, read_some(_, passwordLength)).Times(1).WillOnce(Return());
//EXPECT_CALL(socket, write(_, sizeof(bool))).Times(1).WillOnce(Return());
//
//// Выполняем тестируемый код
//bool is_authenticated = check_password_correct(login, hashPass(password));
//ASSERT_FALSE(is_authenticated);
//}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}