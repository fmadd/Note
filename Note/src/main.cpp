#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include "crpt.h"
#include <stdio.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

using boost::asio::ip::tcp;
using namespace std;
using namespace CryptoPP;
using namespace boost::asio;

string current_user;

//receive file
void except_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey, string user, string filename) { //Hurray!

    int number = 3;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    size_t filename_length = filename.size();
    write(socket, boost::asio::buffer(&filename_length, sizeof(size_t)));
    write(socket, boost::asio::buffer(filename));

    // Создаем файл для сохранения получаемых данных.
    std::filesystem::path current_path = std::filesystem::current_path();

    std::filesystem::path parent_path = current_path.parent_path();

    std::filesystem::path user_path = parent_path / "user_files";
    std::filesystem::create_directory(user_path);


    ofstream file(user_path / filename, ios::out);
    size_t received_bytes = 0;
    ImGui::Text("Create file");
    //cout << "create file" << endl;
    size_t file_size;

    // Получаем длину  файла.
    socket.read_some(boost::asio::buffer(&file_size, sizeof(std::streampos)));

    do {
        // Получаем файл по частям.
        vector<char> buffer(1024);
        size_t bytes_received = socket.read_some(boost::asio::buffer(buffer));

        ImGui::Text("Start writing");
        //cout << "start writing" << endl;
        std::string ciphertext(buffer.begin(), buffer.end());
        std::string plaintext = decryptAES(ciphertext, aesKey);
        file.write(plaintext.c_str(), plaintext.size());
        received_bytes += bytes_received;
        //cout << bytes_received << "Received bytes: " << received_bytes << endl;

    } while (received_bytes < file_size);

    ImGui::Text("File has been sucessfully received");
    // cout << "Файл был успешно получен: " << filename << endl;
}

void delete_file(tcp::socket& socket, CryptoPP::SecByteBlock aesKey) { //Hurray!
    int number = 8;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::string owner, filename;

    ImGui::OpenPopup("Delete File");
    if (ImGui::BeginPopupModal("Delete File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Owner", &owner);
        ImGui::InputText("Filename", &filename);
        ImGui::EndPopup();
    }

    //std::cout << "Введите имя владельца: ";
    //std::cin >> owner;
    //std::cout << "Имя файла: ";
    //std::cin >> filename;

    // Отправляем логин и пароль на сервер.
    std::size_t owner_length = owner.size();
    std::size_t filename_length = filename.size();

    boost::asio::write(socket, boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(owner, owner_length));
    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));

}

void send_file(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey) { //Hurray!
    int number = 4;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    string owner, owner_length;

    ImGui::InputText("Input owner's name", &owner);
   // cout << "Введите имя владельца: ";
    //cin >> owner;
    owner_length = owner.size();
    write(socket, buffer(&owner_length, sizeof(size_t)));
    write(socket, buffer(owner));

    string filename;

    ImGui::InputText("File name", &filename);
    //cout << "Введите имя файла: ";
    //cin >> filename;
    filename = encryptAES(filename, aesKey);

    size_t filename_length = filename.length();
    write(socket, buffer(&filename_length, sizeof(size_t)));
    write(socket, buffer(filename));

    ifstream file;
    file.open("userfiles/" + filename, std::ifstream::in);
    //cout<<file.good()<<" "<<file.is_open()<<" "<<file.eof() <<endl;
    if (!file.is_open()) throw std::runtime_error("File not exist");

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    write(socket, buffer(&file_size, sizeof(std::streampos)));

    //cout<<sizeof(std::streampos)<<endl;

    file.seekg(0, std::ios::beg);

    while (!file.eof()) {
        vector<char> buffer(1008);
        file.read(buffer.data(), buffer.size());
        //cout <<"read sours file "<<endl;
        std::string plaintext(buffer.begin(), buffer.end());
        std::string chipertext = encryptAES(plaintext, aesKey);
        size_t bytes_sent = write(socket, boost::asio::buffer(chipertext));
        //cout << "send "<< bytes_sent<<endl;
    }
    file.close();
    //cout << "Файл был успешно отправлен: " << filename << endl;
}

void authentication(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey) { // Hurray!
    int number = 2;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));


    std::string login, password;
    ImGui::InputText("Login", &login);
    ImGui::InputText("Password", &password);

    //std::cout << "Логин: ";
    //std::cin >> login;
    //std::cout << "Пароль: ";
    //std::cin >> password;

    password = encryptAES(password, aesKey);

    // Отправляем логин и пароль на сервер.
    std::size_t login_length = login.size();
    std::size_t password_length = password.size();

    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
    boost::asio::write(socket, boost::asio::buffer(&password_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(password, password_length));

    // Получаем ответ от сервера.
    bool response;
    socket.read_some(boost::asio::buffer(&response, sizeof(bool)));

    if (!response) {
        throw std::runtime_error("Wrong password or login");
        ImGui::Text("Wrong password or login");
    }
    //std::cout << response << std::endl;
}

void registration(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey) { // Hurray!
    int number = 1;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::string login, password;
    ImGui::InputText("Login", &login);
    ImGui::InputText("Password", &password);

    //std::cout << "Логин: ";
    //std::cin >> login;
    //std::cout << "Пароль: ";
    //std::cin >> password;
    password = encryptAES(password, aesKey);

    // Отправляем логин и пароль на сервер.
    std::size_t login_length = login.size();
    std::size_t password_length = password.size();

    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
    boost::asio::write(socket, boost::asio::buffer(&password_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(password, password_length));

    // Получаем ответ от сервера.
    bool response;
    socket.read_some(boost::asio::buffer(&response, sizeof(bool)));

    if (ImGui::Button("Sign Up") && response) {
        ImGui::Begin("Your menu");
        // cheto napisat'
        ImGui::Text("You are now sucessfully registered");
        ImGui::End();
    }

    if (!response)
    {
        throw std::runtime_error("Неудалось зарегестрировать пользователя");
        ImGui::Text("We can't register this user");

    }
    //std::cout << response << std::endl;

}

void delete_user(tcp::socket& socket) { //Hurray!
    int number = 7;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));
    std::string login;

    ImGui::OpenPopup("Delete user");
    if (ImGui::BeginPopupModal("Delete User", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Username", &login);
        ImGui::EndPopup();
    }

    //std::cout << "Кого удаляем : ";
    //std::cin >> login;

    std::size_t login_length = login.size();
    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));

}

void add_access(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey) { //Hurray!

    int number = 5;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::string filename, user;

    ImGui::OpenPopup("Add Access");
    if (ImGui::BeginPopupModal("Add Access", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Filename", &filename);
        ImGui::InputText("User", &user);
        ImGui::EndPopup();
    }
    //std::cout << " Имя файла: ";
    //std::cin >> filename;
    //std::cout << "Кому даем права: ";
    //std::cin >> user;

    std::size_t filename_length = filename.size();
    std::size_t user_length = user.size();

    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename, filename_length));
    boost::asio::write(socket, boost::asio::buffer(&user_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(user, user_length));

}

void log_out(tcp::socket& socket) { //Hurray!
    int number = 6;
    ImGui::Text("You are now logged out");
    //cout << "Logout" << endl;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

}

CryptoPP::SecByteBlock get_aesKey(tcp::socket& socket) {
    DH dh;
    AutoSeededRandomPool rnd;
    SecByteBlock privKey, pubKey, otherPubKey;
    GenerateDHKeys(socket, dh, rnd, privKey, pubKey);
    send_crypto_block(socket, pubKey);
    otherPubKey = receive_crypto_block(socket);

    SecByteBlock sharedKey(dh.AgreedValueLength());
    if (!dh.Agree(sharedKey, privKey, otherPubKey))
        throw std::runtime_error("Ошибка шифрования");

    CryptoPP::SecByteBlock aesKey = deriveAESKeyFromDH(sharedKey);
    return aesKey;
}

void download_file(const std::string& file) {
    std::ofstream save_file("saved_" + file);
    if (!save_file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
}

int main() {
    try {
        io_service io_service;
        tcp::socket socket(io_service);
        socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 1234));

        CryptoPP::SecByteBlock aesKey = get_aesKey(socket);
        registration(socket, aesKey);

        // Initialize GLFW
        glfwInit();
        GLFWwindow* window = glfwCreateWindow(1280, 720, "File Client", NULL, NULL);
        glfwMakeContextCurrent(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        //bool state
        std::string login, password;
        int file_count = 0;
        bool userExists = false;
        bool userAlreadyOnPlatform = false;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Sign Up");

            if (ImGui::Button("Sign In")) {           
                if (authentication()) {
                    if (userAlreadyOnPlatform) {
                        ImGui::Text("User is already on the platform");
                    } else {
                        ImGui::Begin("Your menu");
                        ImGui::Text("Welcome to your menu!");
                        ImGui::End();
                        ImGui::Begin("Workspace");

                        userAlreadyOnPlatform = true; // where is the already on a platgorm check??
                        if (ImGui::BeginTabBar("Files")) {
                            //might be a problem with user_files as it's not declared it is 
                            for (const auto& file : user_files) {
                                if (ImGui::BeginTabItem(file.c_str())) {
                                    std::string file_content = read_file(file);
                                    if (is_text_file(file)) {
                                        ImGui::Text(file_content.c_str());
                                    }
                                    else {
                                        ImGui::Text("Can't open this format, please download the file");
                                    }

                                    if (ImGui::Button("Download file")) {
                                        download_file();
                                    }

                                    ImGui::EndTabItem();
                                }
                            }
                            ImGui::EndTabBar();
                        }

                        if (ImGui::Button("Add access")) {
                            add_access();
                        }

                        if (ImGui::Button("Send File")) {
                            send_file();
                        }

                        if (ImGui::Button("Delete file")) {
                            delete_file();
                        }

                        if (ImGui::Button("Delete user")) {
                            delete_user();
                        }

                        if (ImGui::Button("Sign out")) {
                            log_out();
                            userAlreadyOnPlatform = false;
                            // end!! 
                        }

                        ImGui::End();

                    }
                } else {
                    ImGui::Text("Invalid login or password");
                }
            }

            ImGui::End();

            ImGui::EndFrame();
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();

        socket.close();
    }
    catch (exception& e) {
        cerr << "Mistake: " << e.what() << endl;
    }

    return 0;
}
