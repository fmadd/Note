#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include "crpt.h"
#include <stdio.h>
//#include <window.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "glfw/include/GLFW/glfw3.h"
#include <string>
#include <fstream>

#include "utils.hpp"

using boost::asio::ip::tcp;
using namespace std;
using namespace CryptoPP;
using namespace boost::asio;


enum LOGIN_STATE {
    UNKNOWN,
    SUCCESS,
    FAILURE
};

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
std::string fileContent;
int clicked = 0;
bool fileContentLoaded = false;

void start_menu() {
    ImGui::Begin("Your menu");
    ImGui::Text("Welcome to your menu!");
    ImGui::End();
}

int main() {
    try {
        io_service io_service;
        tcp::socket socket(io_service);
        socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 1234));
        CryptoPP::SecByteBlock aesKey = get_aesKey(socket);

        if (!glfwInit())
            return 1;

        GLFWwindow *window = glfwCreateWindow(1280, 720, "File Client", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            return 1;
        }
        glfwMakeContextCurrent(window);

        // Initializing ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        //bool state
        LOGIN_STATE login_state = LOGIN_STATE::UNKNOWN;
        static char login[128] = "";
        static char password[128] = "";
        static char filename[128] = "";
        static char user[128] = "";
        static char owner[128] = "";
//        std::string read_file(const std::string & file);
//        bool is_text_file(const std::string & file);

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Main Menu");
                ImGui::Text("This is the main menu");
                if (ImGui::Button("Sign Up")) {
                    ImGui::OpenPopup("Sign Up");
                }
                if (ImGui::BeginPopupModal("Sign Up", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    //ImGui::InputText("Login", loginBuf, sizeof(loginBuf), ImGuiInputTextFlags_CharsUppercase, const_cast<char*>(login.c_str()));
                    ImGui::InputText("Login", login, IM_ARRAYSIZE(login));
                    ImGui::InputText("Password", password, IM_ARRAYSIZE(password));
                    if (ImGui::Button("Register")) {
                        registration(socket, aesKey, login, password);
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                if (ImGui::Button("Sign In")) { //click++
                    ImGui::OpenPopup("Sign In");
                }
                if (ImGui::BeginPopupModal("Sign In", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    ImGui::InputText("Login", login, IM_ARRAYSIZE(login));
                    ImGui::InputText("Password", password, IM_ARRAYSIZE(password));
                    if (ImGui::Button("Log in")) {
                        // authentication(socket, aesKey, login, password);
                        login_state = LOGIN_STATE::SUCCESS;
                        // if authentication was failed set login_state = LOGIN_STATE::FAILURE
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                if (login_state == LOGIN_STATE::SUCCESS) {
                    start_menu();
                    {
                        ImGui::SetNextWindowSize({400, 400});
                        ImGui::Begin("Workspace");
                        ImGui::Text("User info (...)");

                        if (ImGui::BeginTabBar("userfiles")) {
                            if (ImGui::BeginTabBar("userfiles")) {
                                for (int i = 0; i < 10; i++) {
                                    if (ImGui::BeginTabItem(("User's file " + std::to_string(i)).c_str())) {
                                        if (!fileContentLoaded) {
                                            std::ifstream file("userfiles/file_" + std::to_string(i) + ".txt");
                                            if (file.is_open()) {
                                                std::string line;
                                                while (std::getline(file, line)) {
                                                    fileContent += line + "\n";
                                                }
                                                file.close();
                                                fileContentLoaded = true;
                                                ImGui::InputText("File: ", fileContent, sizeof(fileContent));
                                            } else {
                                                ImGui::Text("Failed to open file.");
                                            }
                                        }
                                        ImGui::EndTabItem();
                                    }
                                }

                            }
                            ImGui::EndTabBar();
                        }

                        if (ImGui::BeginTabBar("Actions")) {
                            if (ImGui::Button("Add access")) {
                                memset(filename, 0, 128);
                                memset(user, 0, 128);
                                ImGui::OpenPopup("Add Access");
                            }
                            if (ImGui::BeginPopup("Add Access")) {
                                ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
                                ImGui::InputText("User", user, IM_ARRAYSIZE(user));
                                if (ImGui::Button("Add")) {
                                    add_access(socket, aesKey, filename, user);
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }

                            if (ImGui::Button("Send File")) {
                                memset(owner, 0, 128);
                                memset(filename, 0, 128);
                                ImGui::OpenPopup("Send File");
                            }
                            if (ImGui::BeginPopup("Send File")) {
                                ImGui::InputText("Input owner's name", owner, IM_ARRAYSIZE(owner));
                                ImGui::InputText("File name", filename, IM_ARRAYSIZE(filename));
                                if (ImGui::Button("Send")) {
                                    send_file(socket, aesKey, owner, filename);
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }

                            if (ImGui::Button("Delete file")) {
                                memset(owner, 0, 128);
                                memset(filename, 0, 128);
                                ImGui::OpenPopup("Delete File");
                            }
                            if (ImGui::BeginPopup("Delete File")) {
                                ImGui::InputText("Owner", owner, IM_ARRAYSIZE(owner));
                                ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
                                if (ImGui::Button("Delete")) {
                                    delete_file(socket, aesKey, owner, filename);
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }

                            if (ImGui::Button("Delete user")) {
                                memset(login, 0, 128);
                                ImGui::OpenPopup("Delete user");
                            }
                            if (ImGui::BeginPopup("Delete User")) {
                                ImGui::InputText("Username", login, IM_ARRAYSIZE(login));
                                if (ImGui::Button("Delete")) {
                                    delete_user(socket);
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            if (ImGui::Button("Sign out")) {
                                log_out(socket);
                                break;
                            }
                            ImGui::EndTabBar();
                        }

                        ImGui::End();
                    }
                } else if (login_state == LOGIN_STATE::FAILURE) {
                    ImGui::Text("Invalid login or password");
                }


                ImGui::End();
            }
            //ImGui::EndFrame();
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
