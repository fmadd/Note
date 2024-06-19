#include "server.hpp"
#include "crypto.hpp"
using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;

int main() {
    try {
        DB_service db("dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.23.64.1 port=5432");
        socket_service serv_soc;
        serv_soc.socket_init();
        server_service server(serv_soc, db, sharing_key(serv_soc));

        while (true) {
            try {
                server.session();
            } catch (...) {
                continue;
            }
        }
    }
    catch (exception &e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}