#include "imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>


int main()
{
    // Инициализация GLFW
    glfwInit();

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);

    // Инициализация OpenGL
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Создание контекста OpenGL
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Включение VSync

    // Инициализация ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool showMainWindow = true;
    std::string login, password;
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    //std::ifstream file("file.txt");
    //std::string fileContent;
    int clicked = 0;
    bool fileContentLoaded = false;
    char loginBuffer[256] = "";
    char passwordBuffer[256] = "";
    //bool tab_bar_flags = true;

    // Основной цикл
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Начало кадра ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (showMainWindow)
        {
            ImGui::Begin("Sign in");
            std::string login, password;
            ImGui::InputText("Login", loginBuffer, IM_ARRAYSIZE(loginBuffer), ImGuiInputTextFlags_CallbackAlways, [](ImGuiInputTextCallbackData* data) {
                if (data->EventFlag == ImGuiInputTextFlags_CallbackAlways)
                {
                    ((std::string*)data->UserData)->assign(data->Buf);
                }
                return 0;
                }, &login);
            ImGui::InputText("Password", passwordBuffer, IM_ARRAYSIZE(passwordBuffer), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CallbackAlways, [](ImGuiInputTextCallbackData* data) {
                if (data->EventFlag == ImGuiInputTextFlags_CallbackAlways)
                {
                    ((std::string*)data->UserData)->assign(data->Buf);
                }
                return 0;
                }, &password);

            //ImGui::Text(login);
            //ImGui::Text(password);

            if (ImGui::Button("Sign In")) // add a check whether it matches
            {
                clicked++;
                if (clicked & 1)
                {

                    if (show_another_window) {
                        static float f = 0.0f;
                        static int counter = 0;
                        ImGui::Begin("User's window");
                        ImGui::Text("My files");
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(130);
                        // Vika's note "I'm going to make a loop here based on the amount of files each person has"

                        //It's a table, when you click a name here it comes
                        if (ImGui::BeginTabBar("Lalala"))
                        {
                            if (ImGui::BeginTabItem("Petya's file"))
                            {
                                if (ImGui::Button("Save", ImVec2(50, 20))) {}
                                if (ImGui::Button("Upload a new file", ImVec2(100, 20))) {}
                                ImGui::Text("My working space:");
                                ImGui::SetCursorPosY(70);
                                //Here we save file
                                ImGui::EndTabItem();
                            }
                            if (ImGui::BeginTabItem("Nastya’s file"))
                            {
                                ImGui::Text("My working space:");
                                ImGui::SetCursorPosY(70);
                                if (ImGui::Button("Save", ImVec2(50, 20))) {}
                                if (ImGui::Button("Upload a new file", ImVec2(100, 20))) {}
                                ImGui::EndTabItem();
                            }
                            if (ImGui::BeginTabItem("B’s file"))
                            {
                                ImGui::Text("My working space:");
                                ImGui::SetCursorPosY(70);
                                if (ImGui::Button("Save", ImVec2(50, 20))) {}
                                if (ImGui::Button("Upload a new file", ImVec2(100, 20))) {}
                                ImGui::EndTabItem();
                            }
                        }
                        ImGui::EndTabBar();
                        ImGui::Separator();

                        //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                        //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                        ImGui::End();
                    }
                }
            }
            ImGui::End();
        }
        // Rendering
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

    // Очистка
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

