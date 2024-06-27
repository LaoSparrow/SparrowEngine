#pragma once

#include "scene.hpp"
#include "game_object.hpp"
#include "input.hpp"
#include "utils.hpp"

#include <set>
#include <functional>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

// TODO: Migrate rendering, updating to Engine::RenderingLoop

namespace SE {

    class GameWindow {
    private:
        static SE::Utils::Lazy<> glad_initialization;
        static std::set<GameWindow*> active_windows;
        static GameWindow* current_active_window;

        static void framebuffer_size_callback(GLFWwindow* x, int width, int height);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        std::shared_ptr<Scene> scene_ptr;

        bool is_closed{};

        friend class Engine;

    public:
        GLFWwindow *glfw_window;
        ImGuiContext *imgui_context;
        std::function<void(GameWindow&)> on_draw_callback;
        int width, height;
        glm::mat4 mat_projection, mat_view;
        SE::Input input_system{};

        explicit GameWindow(const char *title = "OpenGL Window", int width = 800, int height = 600);
        ~GameWindow();

        void make_current_context() const;
        void update();
        void render();
        void close();

        inline void bind_scene(std::shared_ptr<Scene> scene) {
            scene_ptr = std::move(scene);
        }

        virtual void on_draw();

        static void Initialize(int major_version, int minor_version);
        static void Terminate();

        static inline GameWindow* GetCurrent() {
            return current_active_window;
        }
    };

// namespace Engine
}
