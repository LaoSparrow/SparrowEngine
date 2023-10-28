#include "sparrow_engine/game_window.hpp"

#include <iostream>
#include <set>

namespace SparrowEngine {

    bool GameWindow::is_glad_initialized = false;

    std::set<GameWindow *> GameWindow::active_windows;
    GameWindow* GameWindow::current_active_window;

    void GameWindow::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glfwMakeContextCurrent(window);
        glViewport(0, 0, width, height);
    }

    GameWindow::GameWindow(const char *title, int width, int height) : on_draw_callback(
        [this](const GameWindow &w) -> void { on_draw(); }) {
        glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (glfw_window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            Terminate();
        }
        glfwMakeContextCurrent(glfw_window);
        glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);

        if (!is_glad_initialized) {
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                std::cout << "Failed to Initialize GLAD" << std::endl;
            }
            is_glad_initialized = true;
        }
        glViewport(0, 0, width, height);

        imgui_context = ImGui::CreateContext();
        ImGui::SetCurrentContext(imgui_context);
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
        ImGui_ImplOpenGL3_Init("#version 430");

        active_windows.insert(this);
    }

    GameWindow::~GameWindow() {
        close();
    }

    void GameWindow::make_current_context() const {
        glfwMakeContextCurrent(glfw_window);
//        ImGui::SetCurrentContext(imgui_context);
    }

    void GameWindow::update() {
        make_current_context();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (scene_ptr)
            scene_ptr->update();
        render();
    }

    void GameWindow::render() {
        make_current_context();

        on_draw_callback(*this);
        if (scene_ptr)
            scene_ptr->render();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glfw_window);
    }

    void GameWindow::close() {
        active_windows.erase(this);
        make_current_context();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(imgui_context);
        glfwDestroyWindow(glfw_window);
    }

    void GameWindow::Initialize(int major_version, int minor_version) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void GameWindow::Terminate() {
        glfwTerminate();
    }

    void GameWindow::on_draw() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    GameWindow *GameWindow::GetCurrentActiveWindow() {
        return current_active_window;
    }

}
