#include "base.h"

// Функция для проверки наличия логина в базе
bool check_login_exists(const std::string &login, const std::string &con_string ) {
    pqxx::connection conn(con_string);

    pqxx::work txn(conn);
    const std::string query = "SELECT COUNT(*) FROM users WHERE login = " + txn.quote(login);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result[0][0].as<int>() > 0;
}

// Функция для проверки правильности пароля пользователя
bool check_password_correct(const std::string &login, const std::string &password, const std::string &con_string ) {
    pqxx::connection conn(con_string);

    pqxx::work txn(conn);
    const std::string query = "SELECT COUNT(*) FROM users WHERE login = " + txn.quote(login) + " AND password = " + txn.quote(password);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result[0][0].as<int>() > 0;
}

// Функция для проверки наличия пользователя в базе и удаления его
void delete_user(const std::string &login, const std::string &con_string ) {

    if (check_login_exists(login, con_string)) {
        pqxx::connection conn(con_string);
        pqxx::work txn(conn);
        const std::string deleteQuery = "DELETE FROM users WHERE  login = " + txn.quote(login);
        txn.exec(deleteQuery);
        std::cout << "Пользователь " << login << " удален из базы." << std::endl;
        txn.commit();
        conn.close();
    } else {
       // throw "l";
        std::cout << "Пользователь " << login << " не найден в базе." << std::endl;
    }

}

// Функция для добавления нового пользователя
void add_user(const std::string &login, const std::string &password, const std::string &con_string ) {

    if (!check_login_exists(login, con_string)) {
        pqxx::connection conn(con_string);
        pqxx::work txn(conn);
        const std::string query =
                "INSERT INTO users (login, password) VALUES (" + txn.quote(login) + ", " + txn.quote(password) + ")";
        txn.exec(query);
        txn.commit();
        conn.close();
    } else{
        throw std::runtime_error("login already exist");
    }
}

// Функция для предоставления прав
void add_user_access(const std::string &owner, const std::string &filename, const std::string &user, const std::string &con_string ) {
    pqxx::connection conn(con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query =
            "INSERT INTO public.files (login, path) VALUES (" + txn.quote(user) +  ", " + txn.quote(path) + ")";
    txn.exec(query);
    txn.commit();
    conn.close();
}
// Функция проверки можно ли дать файл
bool check_user_access(const std::string &owner, const std::string &filename, const std::string& user, const std::string &con_string ) {
    pqxx::connection conn(con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "SELECT COUNT(*) FROM files WHERE path = " + txn.quote(path) + " AND login = " + txn.quote(user);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result[0][0].as<int>() > 0;
}


void delete_file(const std::string &owner, const std::string &filename, const std::string &con_string ) {
    pqxx::connection conn(con_string);
    pqxx::work txn(conn);
    std::string path = owner+"/"+filename;
    txn.exec("DELETE FROM editing WHERE filename = " + txn.quote(path));
    txn.exec("DELETE FROM public.files WHERE path = " + txn.quote(path));
    txn.commit();
    conn.close();
}

bool check_editing(const std::string &owner, const std::string &filename, const std::string &con_string ) {
    pqxx::connection conn(con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "SELECT COUNT(*) FROM editing WHERE filename = " + txn.quote(path) + " AND flag = " + txn.quote(false);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result[0][0].as<int>() > 0;
}

void start_editing(const std::string &owner, const std::string &filename, const std::string &con_string ) {

    pqxx::connection conn(con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "UPDATE editing SET flag = 'true' WHERE filename=" + txn.quote(path);

    txn.exec(query);
    txn.commit();
    conn.close();

}
void end_editing(const std::string &owner, const std::string &filename, const std::string &con_string ) {
    pqxx::connection conn(con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "UPDATE editing SET flag = 'false' WHERE filename=" + txn.quote(path);

    txn.exec(query);
    txn.commit();
    conn.close();

}

void fetch_editing(const std::string &owner, const std::string &filename, const std::string &con_string ) {
    pqxx::connection conn(con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "SELECT COUNT(*) FROM editing WHERE filename = " + txn.quote(path);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    if (result[0][0].as<int>() > 0){
        end_editing(owner, filename);
    }else{
        pqxx::connection conn(con_string);
        pqxx::work txn(conn);
        const std::string query =
                "INSERT INTO editing (filename, flag) VALUES (" + txn.quote(path) +  ", " + txn.quote(false) + ")";
        txn.exec(query);
        txn.commit();
        conn.close();
    }
}


//int main() {}
//    try {
//        pqxx::connection conn(con_string);
//
//        // Пример использования функций
//        if (check_login_exists(conn, "testuser")) {
//            std::cout << "Логин существует в базе." << std::endl;
//        } else {
//            std::cout << "Логин не существует в базе." << std::endl;
//        }
//
//        if (check_password_correct(conn, "testuser", "testpassword")) {
//            std::cout << "Пароль введен правильно." << std::endl;
//        } else {
//            std::cout << "Неправильный пароль." << std::endl;
//        }
//
//        delete_user(conn, "newuser");
//        add_user(conn, "newuser", "newpassword");
//
//        if (check_password_correct(conn, "newuser", "newpassword")) {
//            std::cout << "Пароль введен правильно." << std::endl;
//        } else {
//            std::cout << "Неправильный пароль." << std::endl;
//        }
//
//        if (findFileForUser(conn, "testuser", "example.txt")) {
//            std::cout << "Файл найден для пользователя." << std::endl;
//        } else {
//            std::cout << "Файл не найден для пользователя." << std::endl;
//        }
//
//       // pqxx::PQfinish(conn);
//    } catch (const std::exception &e) {
//        std::cerr << "Ошибка: " << e.what() << std::endl;
//        return 1;
//    }
//
//    return 0;
//}


