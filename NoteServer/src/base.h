#pragma once
#include <iostream>
#include <pqxx/pqxx>

/**
 * \file base.h
 * \brief Описание класса DB_service
 */

/**
 * \class DB_service
 * \brief Класс, предоставляющий сервисы для работы с базой данных
 */
class DB_service{
 public:
    DB_service(){};
    explicit DB_service(const std::string &con_string) : _con_string(con_string)  {};
    /**
    * \brief Проверяет наличие логина в базе данных.
    * \param login Логин пользователя.
    * \return true, если логин существует в базе данных, иначе false.
    */
    virtual bool check_login_exists(const std::string &login);
    /**
     * \brief Проверяет правильность пароля пользователя.
     * \param login Логин пользователя.
     * \param password Пароль пользователя.
     * \return true, если пароль правильный, иначе false.
     */
    virtual bool check_password_correct(const std::string &login, const std::string &password);
    /**
    * \brief Добавляет нового пользователя в базу данных.
    * \param login Логин пользователя.
    * \param password Пароль пользователя.
    * \throw std::runtime_error, если логин уже существует.
    */
    virtual void add_user(const std::string &login, const std::string &password);
    /**
    * \brief Проверяет наличие пользователя в базе данных и удаляет его.
    * \param login Логин пользователя.
    */
    virtual void delete_user(const std::string &login);
    /**
     * \brief Удаляет файл из базы данных.
     * \param owner Владелец файла.
     * \param filename Имя файла.
     */
    void delete_file(const std::string &owner, const std::string &filename);
    /**
     * \brief Получает информацию о редактировании файла и выполняет соответствующие действия.
     * \param owner Владелец файла.
     * \param filename Имя файла.
     */
    void fetch_editing(const std::string &owner, const std::string &filename);
    /**
     * \brief Завершает редактирование файла.
     * \param owner Владелец файла.
     * \param filename Имя файла.
     */
    void end_editing(const std::string &owner, const std::string &filename);
    /**
     * \brief Начинает редактирование файла.
     * \param owner Владелец файла.
     * \param filename Имя файла.
     */
    void start_editing(const std::string &owner, const std::string &filename);
    /**
    * \brief Проверяет, выполняется ли редактирование файла.
    * \param owner Владелец файла.
    * \param filename Имя файла.
    * \return true, если редактирование файла выполняется, иначе false.
    */
    bool check_editing(const std::string &owner, const std::string &filename);
    /**
     * \brief Предоставляет права пользователю на доступ к файлу.
     * \param owner Владелец файла.
     * \param filename Имя файла.
     * \param user Пользователь, которому предоставляются права.
     */
    void add_user_access(const std::string &owner, const std::string &filename, const std::string &user);
    /**
     * \brief Проверяет доступ пользователя к определенному файлу.
     * \param owner Владелец файла.
     * \param filename Имя файла.
     * \param user Пользователь.
     * \return true, если пользователь имеет доступ к файлу, иначе false.
     */
    bool check_user_access(const std::string &owner, const std::string &filename, const std::string& user);
 private:
    const std::string _con_string;

};

