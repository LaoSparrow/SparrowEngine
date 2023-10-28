#include "sparrow_engine/engine.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/time.hpp"
#include "sparrow_engine/logger.hpp"

#include <GLFW/glfw3.h>
#include <set>


namespace SparrowEngine {

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

                w->update();
            }

            for (auto w : should_close_windows) {
                w->close();
            }
            should_close_windows.clear();

            glfwPollEvents();
        }
    }

}