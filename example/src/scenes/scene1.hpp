#pragma once

#include "sparrow_engine/engine.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/scene.hpp"
#include "sparrow_engine/time.hpp"
#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/components/mesh.hpp"
#include "sparrow_engine/components/camera.hpp"

#include "../scripts/camera_movement.hpp"

#include <iostream>

class BackgroundObject : public SparrowEngine::GameObject {
public:
    BackgroundObject() : GameObject("background object") {}

    void render() override {
        double now = SparrowEngine::Time::GetTimeNow();
        glClearColor(sin(now) * 0.5f + 0.5f, cos(now) * 0.5f + 0.5f, sin(now * 2) * 0.5f + 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SparrowEngine::GameObject::render();
    }
};

class TransformModification : public SparrowEngine::Behavior {
public:
    using SparrowEngine::Behavior::Behavior;

    void update() override {
        auto parent_obj = game_object.lock();
        if (ImGui::Begin((parent_obj->name + " transform").c_str())) {
            ImGui::SliderFloat3("position", (float *)&parent_obj->transform.position, -5.0f, 5.0f);
            ImGui::SliderFloat3("scale", (float *)&parent_obj->transform.scale, 0.0f, 5.0f);
            ImGui::SliderFloat3("rotation", (float *)&parent_obj->transform.rotation, -360.0f, 360.0f);

            ImGui::End();
        }
    }
};

class Scene1 : public SparrowEngine::Scene {
    void start() override {
//        scene_root.children.emplace_back(std::make_shared<BackgroundObject>());
        scene_root.children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("Plane")
                ->add_component<TransformModification>()
                ->add_component<SparrowEngine::Components::Mesh,
                    std::initializer_list<SparrowEngine::Components::MeshData>,
                    std::initializer_list<unsigned int>>(
                    {
                        {{0.5f,  0.5f,  0.0f}, {1.0f, 1.0f}},   // 右上
                        {{0.5f,  -0.5f, 0.0f}, {1.0f, 0.0f}},   // 右下
                        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},   // 左下
                        {{-0.5f, 0.5f,  0.0f}, {0.0f, 1.0f}}    // 左上
                    }, {
                        0, 1, 3, // 第一个三角形
                        1, 2, 3  // 第二个三角形
                    })
                ->configure_component<SparrowEngine::Components::Mesh>([](auto m) -> void {
                    m->use_shader("../example/shaders/transform_shader.vs.glsl", "../example/shaders/double_texture_shader.fs.glsl")
                        ->set_texture(0, "../example/textures/container.jpg")
                        ->set_texture(1, "../example/textures/awesomeface.png");
                })
        );
        scene_root.children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("Cube")
                ->add_component<TransformModification>()
                ->add_component<SparrowEngine::Components::Mesh,
                    std::initializer_list<SparrowEngine::Components::MeshData>>(
                    {
                        {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},
                        {{ 0.5f, -0.5f, -0.5f},  {1.0f, 0.0f}},
                        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
                        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
                        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
                        {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},

                        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
                        {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
                        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
                        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
                        {{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f}},
                        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},

                        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
                        {{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
                        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
                        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
                        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
                        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

                        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
                        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
                        {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
                        {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
                        {{ 0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
                        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

                        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
                        {{ 0.5f, -0.5f, -0.5f},  {1.0f, 1.0f}},
                        {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
                        {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
                        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
                        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},

                        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
                        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
                        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
                        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
                        {{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f}},
                        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}}
                    })
                ->configure_component<SparrowEngine::Components::Mesh>([](auto m) -> void {
                    m->use_shader("../example/shaders/transform_shader.vs.glsl", "../example/shaders/double_texture_shader.fs.glsl")
                        ->set_texture(0, "../example/textures/container.jpg")
                        ->set_texture(1, "../example/textures/awesomeface.png");
                })
        );
        scene_root.children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("Camera Object")
                ->configure_object([](auto o) -> void {
                    o->transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
                })
//                ->add_component<TransformModification>()
                ->add_component<SparrowEngine::Components::Camera>()
                ->add_component<CameraMovement>()
        );

        Scene::start();
    }

    void update() override {
        GLFWwindow *w = SparrowEngine::GameWindow::GetCurrentActiveWindow()->glfw_window;
        if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            std::cout << "Closing current window..." << std::endl;
            glfwSetWindowShouldClose(w, true);
        }
        SparrowEngine::Scene::update();
    }
};
