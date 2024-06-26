#include "sparrow_engine/engine.hpp"

#include <iostream>

using namespace SparrowEngine;

void Engine::Initialize(SparrowEngineConfig config) {
    GameWindow::Initialize(config.opengl.major_version, config.opengl.minor_version);
    Time::Initialize();
}

void Engine::RenderingLoop() {
    Logger::Initialize();

    std::set<GameWindow*> should_close_windows;
    while (!GameWindow::active_windows.empty()) {
        glfwPollEvents();
        Time::Update();
        for (auto w : GameWindow::active_windows) {
            GameWindow::current_active_window = w;
            if (glfwWindowShouldClose(w->glfw_window)) {
                should_close_windows.insert(w);
                continue;
            }

            Scene::current_scene = w->scene_ptr;
            w->update();
            if (Scene::next_scene) {
                w->bind_scene(Scene::next_scene);
                Scene::next_scene = nullptr;
            }
        }

        for (auto w : should_close_windows) {
            w->close();
        }
        should_close_windows.clear();

        std::cout << std::flush;
    }
}