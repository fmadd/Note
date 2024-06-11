#pragma once
#include <iostream>
#include <pqxx/pqxx>

class DB_service{
 public:
    explicit DB_service(const std::string &con_string) : _con_string(con_string)  {};
    bool check_login_exists(const std::string &login);
    bool check_password_correct(const std::string &login, const std::string &password);
    void add_user(const std::string &login, const std::string &password);
    void delete_user(const std::string &login);
    void delete_file(const std::string &owner, const std::string &filename);
    void fetch_editing(const std::string &owner, const std::string &filename);
    void end_editing(const std::string &owner, const std::string &filename);
    void start_editing(const std::string &owner, const std::string &filename);
    bool check_editing(const std::string &owner, const std::string &filename);
    void add_user_access(const std::string &owner, const std::string &filename, const std::string &user);
    bool check_user_access(const std::string &owner, const std::string &filename, const std::string& user);
 private:
    const std::string _con_string;

};

