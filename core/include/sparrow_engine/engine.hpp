#pragma once

#include "sparrow_engine/engine.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/time.hpp"
#include "sparrow_engine/logger.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SparrowEngine {

    struct SparrowEngineConfig {
        struct {
            int major_version;
            int minor_version;
        } opengl;
    };

    class Engine {
    public:

//        static void inline InitializeGLAD() {
//            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
//                std::cout << "Failed to Initialize GLAD" << std::endl;
//            }
//        }

        static void Initialize(SparrowEngineConfig config);

        static void RenderingLoop();
    };

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