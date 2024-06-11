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

// Мок класс для socket
class MockSocket : public socket_service {
public:
    MOCK_METHOD(std::size_t, read_filesize, ());
};

class MockDB : public DB_service {
public:
    MOCK_METHOD(void, delete_user, (const std::string &login));
};


TEST(AuthenticationTest, Success) {

    MockSocket socket_service();
//    MockDB db("dbname=test_db user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
    MockDB db();
    server_service server(socket_service, db, sharing_key(socket_service));

    ASSERT_TRUE(true == false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}