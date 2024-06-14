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

void send_file(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& owner, const std::string& filename) {
    int number = 4;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::size_t owner_length = owner.size();
    boost::asio::write(socket, boost::asio::buffer(&owner_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(owner, owner_length));

    filename = encryptAES(filename, aesKey);

    std::size_t filename_length = filename.size();
    boost::asio::write(socket, boost::asio::buffer(&filename_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(filename));

    ifstream file;
    file.open("userfiles/" + filename, std::ifstream::in);
    if (!file.is_open()) throw std::runtime_error("File not exist");

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    boost::asio::write(socket, boost::asio::buffer(&file_size, sizeof(std::streampos)));

    file.seekg(0, std::ios::beg);

    while (!file.eof()) {
        vector<char> buffer(1008);
        file.read(buffer.data(), buffer.size());
        std::string plaintext(buffer.begin(), buffer.end());
        std::string ciphertext = encryptAES(plaintext, aesKey);
        size_t bytes_sent = boost::asio::write(socket, boost::asio::buffer(ciphertext));
    }
    file.close();
}

void authentication(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& login, const std::string& password) {
    int number = 2;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

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
}

void registration(tcp::socket& socket, CryptoPP::SecByteBlock& aesKey, const std::string& login, const std::string& password) {
    int number = 1;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::size_t login_length = login.size();
    std::size_t password_length = password.size();

    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
    boost::asio::write(socket, boost::asio::buffer(&password_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(password, password_length));

    // Получаем ответ от сервера.
    bool response;
    socket.read_some(boost::asio::buffer(&response, sizeof(bool)));

    if (response) {
        ImGui::Begin("Your menu");
        ImGui::Text("You are now sucessfully registered");
        ImGui::End();
    }

    if (!response) {
        throw std::runtime_error("Неудалось зарегестрировать пользователя");
        ImGui::Text("We can't register this user");
    }
}

void delete_user(tcp::socket& socket, const std::string& login) {
    int number = 7;
    boost::asio::write(socket, boost::asio::buffer(&number, sizeof(number)));

    std::size_t login_length = login.size();
    boost::asio::write(socket, boost::asio::buffer(&login_length, sizeof(std::size_t)));
    boost::asio::write(socket, boost::asio::buffer(login, login_length));
}

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
        char loginBuf[256];
        char passwordBuf[256];
        char filenameBuf[256];
        char userBuf[256];
        char ownerBuf[256];
        std::string login;
        std::string password;
        std::string filename;
        std::string user;
        std::string owner;
        std::string read_file(const std::string & file);
        bool is_text_file(const std::string & file);


        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (ImGui::Button("Sign Up")) {
                ImGui::OpenPopup("Sign Up");
                if (ImGui::BeginPopupModal("Sign Up", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    ImGui::InputText("Login", &login);
                    ImGui::InputText("Password", &password.c_str());
                    ImGui::EndPopup();
                    registration(socket, aesKey, login, password);
                }
            }

            if (ImGui::Button("Sign In")) {
                ImGui::OpenPopup("Sign In");
                if (ImGui::BeginPopupModal("Sign In", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    ImGui::InputText("Login", &login);
                    ImGui::InputText("Password", &password);
                    ImGui::EndPopup();
                }
                authentication(socket, aesKey, login, password);
                {
                    //if (userAlreadyOnPlatform) {
                    //    ImGui::Text("User is already on the platform");
                //}
                //else {
                    ImGui::Begin("Your menu");
                    ImGui::Text("Welcome to your menu!");
                    ImGui::End();
                //}
                    ImGui::Begin("Workspace");

                        //userAlreadyOnPlatform = true; // where is the already on a platgorm check??
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
                            ImGui::OpenPopup("Add Access");
                            if (ImGui::BeginPopupModal("Add Access", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                                ImGui::InputText("Filename", &filename);
                                ImGui::InputText("User", &user);
                                ImGui::EndPopup();
                            }
                            add_access(socket, aesKey, filename, user);
                        }

                        if (ImGui::Button("Send File")) {
                            ImGui::OpenPopup("Send File");
                            if (ImGui::BeginPopupModal("Send File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                                ImGui::InputText("Input owner's name", &owner);
                                ImGui::InputText("File name", &filename);
                                ImGui::EndPopup();
                            }
                            send_file(socket, aesKey, owner, filename);
                        }

                        if (ImGui::Button("Delete file")) {
                            ImGui::OpenPopup("Delete File");
                            if (ImGui::BeginPopupModal("Delete File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                                ImGui::InputText("Owner", &owner);
                                ImGui::InputText("Filename", &filename);
                                ImGui::EndPopup();
                            }
                            delete_file(socket, aesKey, owner, filename);
                        }
                        
                        if (ImGui::Button("Delete user")) {
                            ImGui::OpenPopup("Delete user");
                            if (ImGui::BeginPopupModal("Delete User", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                                ImGui::InputText("Username", &login);
                                ImGui::EndPopup();
                            }
                            delete_user(socket, login);
                        }

                        if (ImGui::Button("Sign out")) {
                            log_out(socket);
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
