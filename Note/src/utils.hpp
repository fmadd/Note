#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include <string>
#include <fstream>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "glfw/include/GLFW/glfw3.h"
#include <string>
#include <fstream>

#include "crpt.h"

using boost::asio::ip::tcp;
using namespace std;
using namespace CryptoPP;
using namespace boost::asio;
namespace fs = std::filesystem;


string current_user;


/**
 * @brief Registers a user by sending registration data securely over a TCP socket.
 *
 * This function registers a user. It first sends a registration number to indicate
 * the type of operation. The password is encrypted using AES encryption with the
 * given AES key.
 *
 * @param socket The TCP socket used for communication.
 * @param aesKey Reference to a CryptoPP::SecByteBlock containing the AES key used for encryption.
 * @param login The user's login or username as a std::string.
 * @param password_plain The user's password in plain text as a std::string.
 * @return Returns true if registration succeeds, false otherwise.
 *         The function also sets the global variable `current_user` to `login` upon successful registration.
 *
 * @details
 * The function performs the following steps:
 * - Sends a registration number (integer) to the server via the socket.
 * - Encrypts the plain text password using AES encryption with `aesKey`.
 * - Sends the length of the login and encrypted password as size_t values followed by
 *   the actual login and password data over the socket.
 * - Reads a boolean response from the socket indicating the success of the registration.
 * - If registration is successful (response is true), updates the global `current_user` variable.
 *
 * @note
 * - This function assumes the socket is already connected and operational.
 */
bool registration(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& login, const std::string& password_plain) {
    int number = 1;

    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::string password = encryptAES(password_plain, aesKey);

    std::size_t login_length = login.size();
    std::size_t password_length = password.size();

    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
    boost::asio::write(socket, boost::asio::buffer(&password_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(password, password_length));
    bool response;
    socket.read_some(boost::asio::buffer(&response, sizeof(bool)));
    if(response) current_user = login;
    return response;
}
/**
 * @brief Authenticates a user by sending authentication data securely over a TCP socket.
 *
 * This function authenticates a user by sending their login and encrypted password
 * over the provided TCP socket. It first sends an authentication number to indicate
 * the type of operation. The password is encrypted using AES encryption with the
 * given AES key.
 *
 * @param socket The TCP socket used for communication.
 * @param aesKey Reference to a CryptoPP::SecByteBlock containing the AES key used for encryption.
 * @param login The user's login or username as a std::string.
 * @param password_plain The user's password in plain text as a std::string.
 * @return Returns true if authentication succeeds, false otherwise.
 *         The function also sets the global variable `current_user` to `login` upon successful authentication.
 *
 * @details
 * The function performs the following steps:
 * - Sends an authentication number (integer) to the server via the socket.
 * - Encrypts the plain text password using AES encryption with `aesKey`.
 * - Sends the length of the login and encrypted password as size_t values followed by
 *   the actual login and password data over the socket.
 * - Reads a boolean response from the socket indicating the success of the authentication.
 * - If authentication is successful (response is true), updates the global `current_user` variable.
 *
 * @note
 * - This function assumes the socket is already connected and operational.
 */
bool authentication(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& login, const std::string& password_plain) {
    int number = 2;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::string password = encryptAES(password_plain, aesKey);

    std::size_t login_length = login.size();
    std::size_t password_length = password.size();

    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
    boost::asio::write(socket, boost::asio::buffer(&password_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(password, password_length));

    bool response;
    socket.read_some(boost::asio::buffer(&response, sizeof(bool)));
    if(response) current_user = login;
    return response;
}
/**
 * @brief Retrieves an encrypted file from the server, decrypts it, and saves it locally.
 *
 * This function requests a file from the server specified by the 'owner' and 'filename'
 * parameters. It sends a request number to indicate the operation type. After receiving
 * the file size from the server, it reads encrypted data in chunks, decrypts it using AES
 * encryption with the provided key, and saves the decrypted file locally in a designated
 * directory.
 *
 * @param socket The TCP socket used for communication.
 * @param aesKey A copy of CryptoPP::SecByteBlock containing the AES key used for decryption.
 * @param owner The owner of the file as a std::string.
 * @param filename The name of the file to retrieve and save as a std::string.
 *
 * @throws std::runtime_error If the file is unavailable or empty (file_size == 0).
 *
 * @details
 * The function performs the following steps:
 * - Sends a request number (integer) to the server via the socket.
 * - Sends the lengths of 'owner' and 'filename' as size_t values followed by the actual
 *   'owner' and 'filename' data over the socket.
 * - Reads the size of the file from the server.
 * - If the file size received is 0, throws a runtime_error indicating that the file is
 *   unavailable or empty.
 * - Retrieves the encrypted file data in chunks from the socket, decrypts it using AES
 *   encryption with 'aesKey', and saves the decrypted data to a file in the "userfiles"
 *   directory relative to the current working directory.
 * - Ensures the "userfiles" directory exists; creates it if it does not.
 *
 * @note
 * - This function assumes the socket is already connected and operational.
 *   file transfers) should be added in a production environment.
 */

void except_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey, string owner, string filename) {

    int number = 3;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::size_t owner_length = owner.size();
    std::size_t filename_length = filename.size();

    boost::asio::write(socket, boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(owner, owner_length));
    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename, filename_length));



    size_t received_bytes = 0;
    size_t file_size;
    socket.read_some(boost::asio::buffer(&file_size, sizeof(std::size_t)));

    if(file_size==0){
        throw std::runtime_error("Файл недоступен или пуст");
    }

    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path parent_path = current_path.parent_path();
    std::filesystem::path user_path = parent_path / "userfiles";
    std::filesystem::create_directory(user_path);
    ofstream file(user_path / filename, ios::out);

    do {
        vector<char> buffer(1024);
        size_t bytes_received = socket.read_some(boost::asio::buffer(buffer));
        //cout << bytes_received<<endl;
        std::string ciphertext(buffer.begin(), buffer.end());
        std::string plaintext = decryptAES(ciphertext, aesKey);

        if(received_bytes + bytes_received - file_size > 0){
            plaintext.resize(file_size - received_bytes);
        }
        file.write(plaintext.c_str(), plaintext.size());
        received_bytes += bytes_received;

    } while (received_bytes < file_size);
}

/**
 * @brief Sends an encrypted file to the server securely over a TCP socket.
 *
 * This function sends a file from the local filesystem to the server. It sends a request
 * number to indicate the operation type, followed by the owner's information and the
 * encrypted filename. The file is then read in chunks, encrypted using AES with the provided
 * key, and sent over the socket.
 *
 * @param socket The TCP socket used for communication.
 * @param aesKey Reference to a CryptoPP::SecByteBlock containing the AES key used for encryption.
 * @param owner The owner of the file as a std::string.
 * @param filename The name of the file to send as a std::string.
 *
 * @throws std::runtime_error If the file does not exist or cannot be opened.
 *
 * @details
 * The function performs the following steps:
 * - Sends a request number (integer) to the server via the socket.
 * - Checks if the specified file exists and can be opened. Throws a runtime_error if the file
 *   does not exist.
 * - Sends the length of 'owner' and the 'owner' data as size_t followed by the actual
 *   'owner' data over the socket.
 * - Encrypts the filename using AES encryption with 'aesKey' and sends its length as size_t
 *   followed by the encrypted filename data over the socket.
 * - Determines the size of the file and sends it to the server.
 * - Reads the file in chunks, encrypts each chunk using AES encryption with 'aesKey', and sends
 *   the encrypted data over the socket until the entire file is sent.
 *
 * @note
 * - This function assumes the socket is already connected and operational.
 *   file transfers) should be added in a production environment.
 * - The file is read from the "../userfiles/" directory relative to the executable's working directory.
 */
void send_file(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& owner, const std::string& filename) {
    int number = 4;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    ifstream file;
    file.open("../userfiles/" + filename, std::ifstream::in);
    if (!file.is_open()) throw std::runtime_error("File not exist");

    std::size_t owner_length = owner.size();
    boost::asio::write(socket, boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(owner, owner_length));

    std::string filename_enc = encryptAES(filename, aesKey);

    std::size_t filename_enc_length = filename_enc.size();
    boost::asio::write(socket, boost::asio::buffer(&filename_enc_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename_enc));



    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    boost::asio::write(socket, boost::asio::buffer(&file_size, sizeof(size_t)));

    file.seekg(0, std::ios::beg);

    while (!file.eof()) {
        vector<char> buffer(1000);
        file.read(buffer.data(), buffer.size());
        std::string plaintext(buffer.begin(), buffer.end());
        std::string ciphertext = encryptAES(plaintext, aesKey);
        std::string pl = decryptAES(ciphertext, aesKey);
        size_t bytes_sent = socket.write_some(boost::asio::buffer(ciphertext));

        //cout <<bytes_sent<<" process\n";
    }
    file.close();
}

/**
 * @brief Grants access to a file for a specific user by sending the necessary data securely over a TCP socket.
 *
 * This function sends a request to the server to grant access to a file for a specified user. It sends a
 * request number to indicate the operation type, followed by the filename and user information.
 *
 * @param socket The TCP socket used for communication.
 * @param aesKey Reference to a CryptoPP::SecByteBlock containing the AES key used for encryption (not used in this function).
 * @param filename The name of the file for which access is being granted as a std::string.
 * @param user The username of the user to whom access is being granted as a std::string.
 *
 * @details
 * The function performs the following steps:
 * - Sends a request number (integer) to the server via the socket to indicate the "grant access" operation.
 * - Sends the length of 'filename' and 'user' as size_t values followed by the actual 'filename' and 'user' data over the socket.
 *
 * @note
 * - This function assumes the socket is already connected and operational.
 * - The AES key parameter 'aesKey' is included for consistency with other functions but is not used in this function.
 */
void add_access(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& filename, const std::string& user) {
    int number = 5;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::size_t filename_length = filename.size();
    std::size_t user_length = user.size();

    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename, filename_length));
    boost::asio::write(socket, boost::asio::buffer(&user_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(user, user_length));
}
/**
 * @brief Logs out the current user by sending a logout request to the server over a TCP socket.
 *
 * This function logs out the current user by sending a request number to indicate the logout operation.
 * It also clears the global `current_user` variable.
 *
 * @param socket The TCP socket used for communication.
 *
 * @details
 * The function performs the following steps:
 * - Sends a request number (integer) to the server via the socket to indicate the logout operation.
 * - Clears the global `current_user` variable to indicate that no user is currently logged in.
 *
 * @note
 * - This function assumes the socket is already connected and operational.
 * - The `current_user` variable is assumed to be a global variable that tracks the currently logged-in user.
 */
void log_out(tcp::socket& socket) {
    int number = 6;
    current_user.resize(0);
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

}

/**
 * @brief Deletes the current user by sending a delete request to the server over a TCP socket.
 *
 * This function deletes the current user by sending a request number to indicate the delete operation.
 * It sends the username of the currently logged-in user to the server and prints a message indicating
 * that the user has been deleted.
 *
 * @param socket The TCP socket used for communication.
 *
 * @details
 * The function performs the following steps:
 * - Sends a request number (integer) to the server via the socket to indicate the delete user operation.
 * - Retrieves the current user's login from the global `current_user` variable.
 * - Prints a message to the console indicating that the current user has been deleted.
 * - Sends the length of the current user's login as a size_t value followed by the actual login data over the socket.
 *
 * @note
 * - This function assumes the socket is already connected and operational.
 * - Proper error handling for socket operations (e.g., connection failures) should be added in a production environment.
 * - The `current_user` variable is assumed to be a global variable that tracks the currently logged-in user.
 */
void delete_user(tcp::socket& socket) {
    int number = 7;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::string login = current_user;
    std::cout << current_user << " удален\n";

    std::size_t login_length = login.size();
    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
}
/**
 * @brief Deletes a specified file for a given owner by sending a delete request to the server over a TCP socket.
 *
 * This function deletes a specified file for a given owner by sending a request number to indicate the delete
 * operation. It sends the owner's information and the filename to the server.
 *
 * @param socket The TCP socket used for communication.
 * @param aesKey A copy of CryptoPP::SecByteBlock containing the AES key used for encryption (not used in this function).
 * @param owner The owner of the file as a std::string.
 * @param filename The name of the file to be deleted as a std::string.
 *
 * @details
 * The function performs the following steps:
 * - Sends a request number (integer) to the server via the socket to indicate the delete file operation.
 * - Sends the length of 'owner' and 'filename' as size_t values followed by the actual 'owner' and 'filename' data over the socket.
 *
 * @note
 * - This function assumes the socket is already connected and operational.
 * - Proper error handling for socket operations (e.g., connection failures) should be added in a production environment.
 * - The AES key parameter 'aesKey' is included for consistency with other functions but is not used in this function.
 */
void delete_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey, const std::string& owner, const std::string& filename) {
    int number = 8;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::size_t owner_length = owner.size();
    std::size_t filename_length = filename.size();

    boost::asio::write(socket, boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(owner, owner_length));
    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename, filename_length));
}

/**
 * @brief Counts the number of files with a specific extension in a given directory.
 *
 * This function iterates through the files in the specified directory and counts how many of them
 * have the specified file extension.
 *
 * @param directory The path to the directory to be searched as a std::string.
 * @param extension The file extension to count as a std::string.
 * @return The number of files with the specified extension in the directory as an int.
 *
 * @details
 * The function performs the following steps:
 * - Initializes a file count to 0.
 * - Iterates through each entry in the specified directory using `fs::directory_iterator`.
 * - Checks if the entry is a regular file and if its extension matches the specified extension.
 * - Increments the file count for each matching file.
 * - Returns the total count of files with the specified extension.
 *
 * @note
 * - This function assumes that the filesystem namespace is aliased as `fs`.
 * - Proper error handling for filesystem operations (e.g., invalid directory path) should be added in a production environment.
 * - The extension parameter should include the dot (e.g., ".txt").
 */
int countFilesInDirectory(const std::string& directory, const std::string& extension) {
    int fileCount = 0;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            ++fileCount;
        }
    }
    return fileCount;
}

/**
 * @brief Reads and displays text files from a specified directory in ImGui tab items.
 *
 * This function reads text files with a specific extension from a given directory, displays each file's content in an ImGui tab item,
 * and allows the user to edit and save the file content through an ImGui input text widget.
 *
 * @details
 * The function performs the following steps:
 * - Defines the directory path (`../userfiles`) and the file extension (`.txt`).
 * - Counts the number of files in the directory with the specified extension using `countFilesInDirectory`.
 * - Iterates through each file, creating an ImGui tab item for each one.
 * - Reads the content of each file into a string and checks if the file exists.
 * - Displays the file content in an ImGui input text multiline widget.
 * - Allows the user to edit the content and saves the changes back to the file.
 * - Displays an error message if the file cannot be opened or saved.
 *
 * @note
 * - This function uses the ImGui library for the graphical user interface.
 * - The function assumes that the ImGui context is already set up and that ImGui is in a state ready for rendering UI elements.
 */
void readFiles() {
    std::string directory = "../userfiles";
    std::string extension = ".txt";
    int fileCount = countFilesInDirectory(directory, extension);

    for (int i = 1; i <= fileCount; i++) {
        if (ImGui::BeginTabItem(("User's file " + std::to_string(i)).c_str())) {
            std::string fileName = directory + "/file_" + std::to_string(i) + extension;
            std::string fileContent;
            bool fileExists = false;

            std::ifstream file(fileName);
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    fileContent += line + "\n";
                }
                file.close();
                fileExists = true;
            }

            if (!fileExists) {
                ImGui::Text("Failed to open file.");
            }

            std::vector<char> inputBuffer(fileContent.begin(), fileContent.end());
            inputBuffer.resize(1024);

            if (ImGui::InputTextMultiline("File", inputBuffer.data(), inputBuffer.size(), ImVec2(400, 200))) {
                std::string newFileContent(inputBuffer.data());

                std::ofstream outFile(fileName, std::ios::trunc);
                if (outFile.is_open()) {
                    outFile << newFileContent;
                    outFile.close();
                } else {
                    ImGui::Text("Failed to save edition.");
                }
            }

            ImGui::EndTabItem();
        }
    }
}

bool deleteFile(const std::string& filename) {
    std::filesystem::path filePath(filename);
    if (std::filesystem::is_regular_file(filePath)) {
        return std::filesystem::remove(filePath);
    }
    return false;
}