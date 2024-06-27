#include "sparrow_engine/game_window.hpp"

#include "fmt/core.h"

#include <ranges>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace SE;

SE::Utils::Lazy<> GameWindow::glad_initialization {[]() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fmt::println("Failed to Initialize GLAD");
    }
}};

std::set<GameWindow *> GameWindow::active_windows;
GameWindow* GameWindow::current_active_window;

void GameWindow::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
    for (auto w : active_windows
        | std::views::filter([&window] (auto x) { return x->glfw_window == window;})) {
        w->width = width;
        w->height = height;
    }
}

void GameWindow::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    for (auto w : active_windows
                  | std::views::filter([&window] (auto x) { return x->glfw_window == window;})) {
        w->input_system.mouse_button_event(button, action, mods);
    }
}

void GameWindow::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    for (auto w : active_windows
                  | std::views::filter([&window] (auto x) { return x->glfw_window == window;})) {
        w->input_system.key_event(key, scancode, action, mods);
    }
}

GameWindow::GameWindow(const char *title, int width, int height)
        : on_draw_callback([this](const GameWindow &w) -> void { on_draw(); }){
    glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (glfw_window == nullptr) {
        fmt::println("Failed to create GLFW window");
        Terminate();
    }
    glfwMakeContextCurrent(glfw_window);
    this->width = width;
    this->height = height;
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
    glfwSetKeyCallback(glfw_window, key_callback);
//        glfwSwapInterval(1);

    glad_initialization.get();

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

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
    ImGui::SetCurrentContext(imgui_context);
}

void GameWindow::update() {
    make_current_context();

    input_system.pre_update();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (Scene::GetCurrent())
        Scene::GetCurrent()->new_frame();
    render();
    input_system.post_update();
}

void GameWindow::render() {
    make_current_context();

    on_draw_callback(*this);
    if (Scene::GetCurrent())
        Scene::GetCurrent()->render();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(glfw_window);
}

void GameWindow::close() {
    if (!is_closed) {
        is_closed = true;
        active_windows.erase(this);
        make_current_context();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(imgui_context);
        glfwDestroyWindow(glfw_window);
    }
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
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
