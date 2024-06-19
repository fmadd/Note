#include "../src/base.h"
#include "doctest.h"
std::string con_string = "dbname=test_db user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432";//"dbname=test_db user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432";

void create_Database() {
        // Создать базу данных, таблицы и вставить данные

        pqxx::connection conn = pqxx::connection(con_string);
        pqxx::work txn(conn);
        txn.exec("CREATE TABLE IF NOT EXISTS users (login TEXT PRIMARY KEY, password TEXT)");
        txn.exec("INSERT INTO users (login, password) VALUES ('John', '30'), ('Jane', '25'), ('test_user', 'test_password')");

        txn.exec("CREATE TABLE IF NOT EXISTS files (id SERIAL PRIMARY KEY, login TEXT, path TEXT)");
        txn.exec("INSERT INTO files (login, path) VALUES ('John', 'John/30'), ('john.doe', 'john.doe/test_file.txt')");

        txn.exec("CREATE TABLE IF NOT EXISTS editing (filename TEXT PRIMARY KEY, flag BOOLEAN)");
        txn.exec("INSERT INTO editing (filename, flag) VALUES ('john.doe/test_file.txt', 'false'), ('Jane/20', 'true')");

        txn.exec("END");
        DB_service db(con_string);
        db.fetch_editing("john.doe","test_file.txt" );
        txn.commit();
        conn.close();

}
void drop_Database() {
        // Удалить таблицу

        pqxx::connection conn = pqxx::connection(con_string);
        pqxx::work txn(conn);
        txn.exec("DROP TABLE IF EXISTS users, files, editing");
        txn.exec("END");
        txn.commit();
        conn.close();
}

TEST_CASE( "TestLogin") {
    // Проверить, что возвращаемый пользователь имеет правильное имя и возраст
    create_Database();
    DB_service db(con_string);

    std::cout << con_string<<std::endl;
    CHECK(db.check_login_exists("John"));
    //CHECK(!check_login_exists("Bob", con_string));
    drop_Database();
}

TEST_CASE( "TestPassword") {
// Проверить, что возвращаемый пользователь имеет правильное имя и возраст
    create_Database();
    DB_service db(con_string);
    CHECK(db.check_password_correct("John", "30"));

    drop_Database();
}

TEST_CASE( "TestAdd") {
    create_Database();
    DB_service db(con_string);
    db.add_user("Nastya1", "40");

    CHECK(db.check_login_exists("Nastya1"));
    drop_Database();
}

TEST_CASE( "TestDel") {
    create_Database();
    DB_service db(con_string);
    CHECK(db.check_login_exists("John"));
    CHECK(!db.check_login_exists("Bob"));
    db.delete_user("John");
    CHECK_THROWS((db.delete_user("John1")));
    CHECK(!db.check_login_exists("John"));
    drop_Database();
}



TEST_CASE("add_user_access") {

    create_Database();
    DB_service db(con_string);
    db.add_user_access("john.doe", "test_file.txt", "test_user");
    CHECK(db.check_user_access("john.doe", "test_file.txt", "test_user") == true);
    CHECK(db.check_user_access("john.doe", "another_file.txt", "test_user") == false);
    drop_Database();

}

TEST_CASE("delete_file") {
    create_Database();
    DB_service db(con_string);
    db.delete_file("john.doe", "test_file.txt");
    CHECK(db.check_user_access("john.doe", "test_file.txt", "test_user") == false);
    drop_Database();
}

TEST_CASE("check_editing") {
    create_Database();
    DB_service db(con_string);
    CHECK(db.check_editing("john.doe", "test_file.txt") == true);
    CHECK(db.check_editing("Jane", "20") == false);

    db.start_editing("john.doe", "test_file.txt");

    CHECK(db.check_editing("john.doe", "test_file.txt") == false);

    db.end_editing("john.doe", "test_file.txt");

    CHECK(db.check_editing("john.doe", "test_file.txt") == true);
    drop_Database();
}

TEST_CASE("fetch_editing") {
    create_Database();
    DB_service db(con_string);
    db.fetch_editing("john.doe", "test_file.txt");
    CHECK(db.check_editing("john.doe", "test_file.txt") == true);
    drop_Database();
}