#pragma once

#include "scene.hpp"

#include <iostream>
#include <set>
#include <functional>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace SparrowEngine {

    class GameWindow {
    private:
        static bool is_glad_initialized;
        static std::set<GameWindow*> active_windows;
        static GameWindow* current_active_window;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        std::shared_ptr<Scene> scene_ptr;

        friend class Engine;

    public:
        GLFWwindow *glfw_window;
        ImGuiContext *imgui_context;
        std::function<void(GameWindow&)> on_draw_callback;

        explicit GameWindow(const char *title = "OpenGL Window", int width = 800, int height = 600);
        ~GameWindow();

        void make_current_context() const;
        void update();
        void render();
        void close();

        inline GameWindow& bind_scene(std::shared_ptr<Scene> scene) {
            scene_ptr = std::move(scene);
            return *this;
        }

        virtual void on_draw();

        static void Initialize(int major_version, int minor_version);
        static void Terminate();

        static GameWindow* GetCurrentActiveWindow();
    };

// namespace Engine
}
