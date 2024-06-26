#include "common.hpp"
#include "scene1.hpp"
#include "scene2.hpp"
#include "scene3.hpp"
#include "scene4.hpp"

#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/input.hpp"
#include <fmt/core.h>

using namespace SparrowEngine::Example::Scenes;
using Input = SparrowEngine::Input;

void Common::ProcessKeys() {
    GLFWwindow *w = SparrowEngine::GameWindow::GetCurrent()->glfw_window;
    if (Input::KeyTriggered(GLFW_KEY_ESCAPE)) {
        fmt::println("Closing current window...");
        glfwSetWindowShouldClose(w, true);
    }
    if (Input::KeyTriggered(GLFW_KEY_1)) {
        fmt::println("Switching to scene 1");
        SparrowEngine::Scene::SwitchTo(std::make_shared<Scene1>());
    }
    if (Input::KeyTriggered(GLFW_KEY_2)) {
        fmt::println("Switching to scene 2");
        SparrowEngine::Scene::SwitchTo(std::make_shared<Scene2>());
    }
    if (Input::KeyTriggered(GLFW_KEY_3)) {
        fmt::println("Switching to scene 3");
        SparrowEngine::Scene::SwitchTo(std::make_shared<Scene3>());
    }
    if (Input::KeyTriggered(GLFW_KEY_4)) {
        fmt::println("Switching to scene 4");
        SparrowEngine::Scene::SwitchTo(std::make_shared<Scene4>());
    }
}
