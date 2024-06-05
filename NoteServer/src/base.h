#pragma once
#include <iostream>
#include <pqxx/pqxx>

bool check_login_exists(const std::string &login, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
bool check_password_correct(const std::string &login, const std::string &password, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
void add_user(const std::string &login, const std::string &password, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
void delete_user(const std::string &login, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
void delete_file(const std::string &owner, const std::string &filename, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
void fetch_editing(const std::string &owner, const std::string &filename, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
void end_editing(const std::string &owner, const std::string &filename, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
void start_editing(const std::string &owner, const std::string &filename, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
bool check_editing(const std::string &owner, const std::string &filename, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
void add_user_access(const std::string &owner, const std::string &filename, const std::string &user, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
bool check_user_access(const std::string &owner, const std::string &filename, const std::string& user, const std::string &con_string =  "dbname=NoteBase user=postgres password=x5wkfUTi=&8w!e5 hostaddr=172.20.208.1 port=5432");
