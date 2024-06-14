#include "base.h"

// Функция для проверки наличия логина в базе
bool DB_service::check_login_exists(const std::string &login ) {
    pqxx::connection conn(_con_string);

    pqxx::work txn(conn);
    const std::string query = "SELECT COUNT(*) FROM users WHERE login = " + txn.quote(login);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result[0][0].as<int>() > 0;
}

// Функция для проверки правильности пароля пользователя
bool DB_service::check_password_correct(const std::string &login, const std::string &password ) {
    pqxx::connection conn(_con_string);

    pqxx::work txn(conn);
    const std::string query = "SELECT COUNT(*) FROM users WHERE login = " + txn.quote(login) + " AND password = " + txn.quote(password);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result[0][0].as<int>() > 0;
}

// Функция для проверки наличия пользователя в базе и удаления его
void DB_service::delete_user(const std::string &login ) {

    if (DB_service::check_login_exists(login)) {
        pqxx::connection conn(_con_string);
        pqxx::work txn(conn);
        const std::string deleteQuery = "DELETE FROM users WHERE  login = " + txn.quote(login);
        txn.exec(deleteQuery);
        std::cout << "Пользователь " << login << " удален из базы." << std::endl;
        txn.commit();
        conn.close();
    } else {
        throw std::runtime_error("Login not in base");
    }

}

// Функция для добавления нового пользователя
void DB_service::add_user(const std::string &login, const std::string &password ) {
    std::cout<<"add_user"<<std::endl;
    if (!DB_service::check_login_exists(login)) {
        pqxx::connection conn(_con_string);
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
void DB_service::add_user_access(const std::string &owner, const std::string &filename, const std::string &user ) {
    pqxx::connection conn(_con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);
    const std::string query =
            "INSERT INTO public.files (login, path) VALUES (" + txn.quote(user) +  ", " + txn.quote(path) + ")";
    txn.exec(query);
    txn.commit();
    conn.close();
}

bool DB_service::check_user_access(const std::string &owner, const std::string &filename, const std::string& user ) {
    pqxx::connection conn(_con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "SELECT COUNT(*) FROM files WHERE path = " + txn.quote(path) + " AND login = " + txn.quote(user);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result[0][0].as<int>() > 0;
}


void DB_service::delete_file(const std::string &owner, const std::string &filename ) {
    pqxx::connection conn(_con_string);
    pqxx::work txn(conn);
    std::string path = owner+"/"+filename;
    txn.exec("DELETE FROM editing WHERE filename = " + txn.quote(path));
    txn.exec("DELETE FROM public.files WHERE path = " + txn.quote(path));
    txn.commit();
    conn.close();
}

bool DB_service::check_editing(const std::string &owner, const std::string &filename ) {
    pqxx::connection conn(_con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "SELECT COUNT(*) FROM editing WHERE filename = " + txn.quote(path) + " AND flag = " + txn.quote(false);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result[0][0].as<int>() > 0;
}

void DB_service::start_editing(const std::string &owner, const std::string &filename ) {

    pqxx::connection conn(_con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "UPDATE editing SET flag = 'true' WHERE filename=" + txn.quote(path);

    txn.exec(query);
    txn.commit();
    conn.close();

}
void DB_service::end_editing(const std::string &owner, const std::string &filename ) {
    pqxx::connection conn(_con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);

    const std::string query = "UPDATE editing SET flag = 'false' WHERE filename=" + txn.quote(path);

    txn.exec(query);
    txn.commit();
    conn.close();

}

void DB_service::fetch_editing(const std::string &owner, const std::string &filename ) {
    pqxx::connection conn(_con_string);
    std::string path = owner+"/"+filename;
    pqxx::work txn(conn);
    const std::string query = "SELECT COUNT(*) FROM editing WHERE filename = " + txn.quote(path);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    if (result[0][0].as<int>() > 0){
        end_editing(owner, filename);
    }else{
        pqxx::connection conn(_con_string);
        pqxx::work txn(conn);
        const std::string query =
                "INSERT INTO editing (filename, flag) VALUES (" + txn.quote(path) +  ", " + txn.quote(false) + ")";
        txn.exec(query);
        txn.commit();
        conn.close();
    }
}



