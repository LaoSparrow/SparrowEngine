#include "sparrow_engine/engine.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/scene.hpp"
#include "sparrow_engine/time.hpp"
#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/components/mesh.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

class BackgroundObject : public SparrowEngine::GameObject {
public:
    BackgroundObject() : GameObject("background object") {

    }
};

class MyScript : public SparrowEngine::Behavior {
public:
    using SparrowEngine::Behavior::Behavior;

    void start() override {
        std::cout << "MyScript start" << std::endl;
    }

    void update() override {
        GLFWwindow *w = SparrowEngine::GameWindow::GetCurrentActiveWindow()->glfw_window;
        if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            std::cout << "Closing current window..." << std::endl;
            glfwSetWindowShouldClose(w, true);
        }
    }

    void render() override {
        ImGui::ShowDemoWindow();

        double now = SparrowEngine::Time::GetTimeNow();
        glClearColor(sin(now) * 0.5f + 0.5f, cos(now) * 0.5f + 0.5f, sin(now * 2) * 0.5f + 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SparrowEngine::Behavior::render();
    }
};

int main() {
    SparrowEngine::Engine::Initialize({
                                          .opengl = {
                                              .major_version = 4,
                                              .minor_version = 5,
                                          }
                                      });
    SparrowEngine::GameWindow window("Engine Example");
    ImGui::SetCurrentContext(window.imgui_context);

    auto scene = std::make_shared<SparrowEngine::Scene>();
    window.bind_scene(scene);
    scene->scene_root.children.emplace_back(
        std::make_shared<BackgroundObject>()
            ->add_component<MyScript>()
            ->add_component<SparrowEngine::Components::Mesh, std::initializer_list<SparrowEngine::Components::MeshData>>(
                {
                    {{-0.5f, -0.5f, 0.0f}},
                    {{0.5f,  -0.5f, 0.0f}},
                    {{0.0f,  0.5f,  0.0f}}
                })
    );

//    SparrowEngine::GameWindow win2("Engine Example win2");
//
//    auto scene2 = std::make_shared<SparrowEngine::Scene>();
//    win2.bind_scene(scene2);
//    scene2->scene_root.children.emplace_back(
//        std::make_shared<BackgroundObject>()
//            ->add_component<MyScript>()
////            ->add_component<SparrowEngine::Components::Mesh, std::initializer_list<SparrowEngine::Components::MeshData>>(
////                {
////                    {{-0.5f, -0.5f, 0.0f}},
////                    {{0.5f,  -0.5f, 0.0f}},
////                    {{0.0f,  0.5f,  0.0f}}
////                })
//    );

    SparrowEngine::Engine::InitializeGLAD();
    SparrowEngine::Engine::RenderingLoop();

    SparrowEngine::GameWindow::Terminate();
    return 0;
}
