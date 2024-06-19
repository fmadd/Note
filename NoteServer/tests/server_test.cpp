#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/crypto.hpp"
#include "../src/base.h"
#include "../src/server.hpp"
#include "../src/socket_service.hpp"

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;

using ::testing::_;
using ::testing::Return;

class MockSocketService : public socket_service {
public:
    MOCK_METHOD(std::string, get_string, (), (override));
    //MOCK_METHOD(size_t, writeInSocket, (boost::asio::const_buffers_1 buffers), (override));
    MOCK_METHOD(size_t, writeInSocket, (boost::asio::mutable_buffers_1 buffers), (override));
};

class MockDBService : public DB_service {
public:
    MOCK_METHOD(void, add_user, (const std::string& login, const std::string& password), (override));
    MOCK_METHOD(void, delete_user, (const std::string& login), (override));
    MOCK_METHOD(bool,  check_login_exists, (const std::string& login), (override));

};

TEST(ServerServiceTest, Registration) {

    MockSocketService mock_socket;
    MockDBService mock_db;

    CryptoPP::AutoSeededRandomPool prng;
    const size_t blockSize = SHA256::DIGESTSIZE;
    CryptoPP::SecByteBlock aesKey(blockSize);
    prng.GenerateBlock(aesKey, aesKey.size());

    server_service server(mock_socket, mock_db, aesKey);

    EXPECT_CALL(mock_socket, get_string())
    .Times(2)
    .WillOnce(testing::Return("test_login"))
    .WillOnce(testing::Return(encryptAES("test_password", aesKey)));
    EXPECT_CALL(mock_socket, writeInSocket(testing::_))
    .Times(1);

    server.registration();
    mock_socket.close();

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}