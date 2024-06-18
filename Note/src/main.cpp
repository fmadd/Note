#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include "crpt.h"
#include <stdio.h>

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
std::string fileContent;
bool fileContentLoaded = false;

void start_menu() {
    ImGui::Begin("Your menu");
    ImGui::Text("Welcome to your menu!");
    ImGui::End();
}
void success_reg() {
    ImGui::Begin("Registration");
    ImGui::Text("You have successfully registered!");
    ImGui::End();
}

void not_reg() {
    ImGui::Begin("Registration");
    ImGui::Text("Could not register this user, please try again");
    ImGui::End();
}

void readFiles() {
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
                } else {
                    ImGui::Text("Failed to open file.");
                }
            }
            if (ImGui::InputTextMultiline("File", &fileContent[0], fileContent.size(), ImVec2(400, 200))) {
                std::ofstream outFile("userfiles/file_" + std::to_string(i) + ".txt");
                if (outFile.is_open()) {
                    outFile << fileContent;
                    outFile.close();
                } else {
                    ImGui::Text("Failed to save edition.");
                }
            }
            ImGui::EndTabItem();
        }
    }
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
                    ImGui::InputText("Login", login, IM_ARRAYSIZE(login));
                    ImGui::InputText("Password", password, IM_ARRAYSIZE(password));
                    if (ImGui::Button("Register")) {
                        bool flag = registration(socket, aesKey, login, password);
                        success_reg();
                        if(!flag) not_reg();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                if (ImGui::Button("Sign In")) {
                    ImGui::OpenPopup("Sign In");
                }
                if (ImGui::BeginPopupModal("Sign In", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    ImGui::InputText("Login", login, IM_ARRAYSIZE(login));
                    ImGui::InputText("Password", password, IM_ARRAYSIZE(password));
                    if (ImGui::Button("Log in")) {
                        bool flag = authentication(socket, aesKey, login, password);
                        login_state = LOGIN_STATE::SUCCESS;
                        if(!flag) login_state = LOGIN_STATE::FAILURE;
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
                                readFiles();
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
                                    ImGui::Text("The user has been successfully added!");
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }

                            if (ImGui::Button("Send File")) {
                                memset(owner, 0, 128);
                                memset(filename, 0, 128);
                                ImGui::OpenPopup("Send File");
                            }
                            if (ImGui::BeginPopup("Send File", ImGuiWindowFlags_AlwaysAutoResize)) {
                                ImGui::InputText("Input owner's name", owner, IM_ARRAYSIZE(owner));
                                ImGui::InputText("File name", filename, IM_ARRAYSIZE(filename));
                                if (ImGui::Button("Send")) {
                                    try {
                                        send_file(socket, aesKey, owner, filename);
                                        ImGui::Text("The file has been successfully sent!");
                                        ImGui::CloseCurrentPopup();
                                    }catch (const std::runtime_error &e) {
                                        ImGui::Begin("Error", ImGuiWindowFlags_MenuBar);
                                        ImGui::Text("File doesn't exist");
                                        ImGui::End();
                                    }
                                }
                                ImGui::EndPopup();
                            }

                            if (ImGui::Button("Download File")) {
                                memset(owner, 0, 128);
                                memset(filename, 0, 128);
                                ImGui::OpenPopup("Download File");
                            }
                            if (ImGui::BeginPopup("Download File")) {
                                ImGui::InputText("Owner", owner, IM_ARRAYSIZE(owner));
                                ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
                                if (ImGui::Button("Download")) {
                                    try {
                                        except_file(socket, aesKey, owner, filename);
                                        ImGui::Text("The file has been successfully downloaded!");
                                        ImGui::CloseCurrentPopup();
                                    }catch (const std::runtime_error &e) {
                                        ImGui::Begin("Error", ImGuiWindowFlags_MenuBar);
                                        ImGui::Text("Don't have access to the file or it is empty");
                                        ImGui::End();
                                    }
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
                                    ImGui::Text("The user has been successfully deleted!");
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            if (ImGui::Button("Sign out")) {
                                log_out(socket);
                                ImGui::Text("You have been successfully logged out!");
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
