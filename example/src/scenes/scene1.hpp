#pragma once

#include "sparrow_engine/engine.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/scene.hpp"
#include "sparrow_engine/time.hpp"
#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/components/mesh.hpp"
#include "sparrow_engine/components/camera.hpp"

#include "glm/gtc/quaternion.hpp"

#include "../scripts/transform_modification.hpp"
#include "../scripts/camera_movement.hpp"
#include "../constants.hpp"

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

class Scene1 : public SparrowEngine::Scene {
    void start() override {
//        scene_root.children.emplace_back(std::make_shared<BackgroundObject>());
        using Mesh = SparrowEngine::Components::Mesh;
        scene_root->children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("Plane")
                ->add_component<TransformModification>()
                ->add_component<SparrowEngine::Components::Mesh>(
                    SparrowEngine::Example::Constants::plane,
                    SparrowEngine::Example::Constants::plane_vertex_indices)
                ->configure_component<SparrowEngine::Components::Mesh>([](auto m) -> void {
                    m->use_shader("../example/shaders/transform_shader.vs.glsl", "../example/shaders/double_texture_shader.fs.glsl")
                        ->set_texture("texture_0", "../example/textures/container.jpg")
                        ->set_texture("texture_1", "../example/textures/awesomeface.png");
                })
        );
        scene_root->children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("Cube")
                ->add_component<TransformModification>()
                ->add_component<SparrowEngine::Components::Mesh>(
                    SparrowEngine::Example::Constants::cube)
                ->configure_component<SparrowEngine::Components::Mesh>([](auto m) -> void {
                    m->use_shader("../example/shaders/transform_shader.vs.glsl", "../example/shaders/double_texture_shader.fs.glsl")
                        ->set_texture("texture_0", "../example/textures/container.jpg")
                        ->set_texture("texture_1", "../example/textures/awesomeface.png");
                })

                ->add_child_object("Cube child")
                ->configure_child_object([](auto obj) -> void {
                    obj->transform.position = glm::vec3(1.0f, 1.0f, 0.0f);
                    obj->transform.rotation *= glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    obj->add_component<TransformModification>();
                    obj->add_component<Mesh>(
                        SparrowEngine::Example::Constants::cube);
                    obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) -> void {
                        m->use_shader("../example/shaders/transform_shader.vs.glsl", "../example/shaders/double_texture_shader.fs.glsl")
                            ->set_texture("texture_0", "../example/textures/container.jpg")
                            ->set_texture("texture_1", "../example/textures/awesomeface.png");
                    });
                })
        );
        scene_root->children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("Camera Object")
                ->configure_object([](auto o) -> void {
                    o->transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
                })
                ->add_component<TransformModification>()
                ->add_component<SparrowEngine::Components::Camera>()
                ->add_component<CameraMovement>()
        );
//        scene_root.children.emplace_back(
//            std::make_shared<SparrowEngine::GameObject>("Camera Parent")
//                ->add_component<TransformModification>()
//
//                ->add_child_object("Camera Object")
//                ->configure_child_object([](auto cam) -> void {
//                    cam->transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
//                    cam->add_component<SparrowEngine::Components::Camera>();
//                    cam->add_component<CameraMovement>();
//                })
//        );
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
