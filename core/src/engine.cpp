#include "sparrow_engine/engine.hpp"

using namespace SparrowEngine;

void Engine::Initialize(SparrowEngineConfig config) {
    GameWindow::Initialize(config.opengl.major_version, config.opengl.minor_version);
    Time::Initialize();
}

void Engine::RenderingLoop() {
    Logger::Initialize();

    std::set<GameWindow*> should_close_windows;
    while (!GameWindow::active_windows.empty()) {
        Time::Update();
        for (auto w : GameWindow::active_windows) {
            GameWindow::current_active_window = w;
            if (glfwWindowShouldClose(w->glfw_window)) {
                should_close_windows.insert(w);
                continue;
            }

            Scene::current_scene = w->scene_ptr;
            w->update();
        }

        for (auto w : should_close_windows) {
            w->close();
        }
        should_close_windows.clear();

        glfwPollEvents();
    }
}