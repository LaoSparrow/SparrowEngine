#pragma once

#include "sparrow_engine/scene.hpp"
#include "sparrow_engine/game_object.hpp"
#include "sparrow_engine/components/mesh.hpp"
#include "sparrow_engine/components/camera.hpp"

#include "glm/gtc/quaternion.hpp"

#include "../constants.hpp"
#include "../scripts/transform_modification.hpp"
#include "../scripts/camera_movement.hpp"

class Scene2 : public SparrowEngine::Scene {
    using SparrowEngine::Scene::Scene;

    void start() override {
        scene_root.children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("light")
                ->configure_object([](auto o) -> void {
                    o->transform.position = glm::vec3(1.2f, 1.0f, 2.0f);
                    o->transform.scale = glm::vec3(0.2f);
                })
                ->add_component<SparrowEngine::Components::Mesh>(SparrowEngine::Example::Constants::cube)
                ->configure_component<SparrowEngine::Components::Mesh>([](auto m) -> void {
                    m->use_shader("../example/shaders/transform_viewspace_shader.vs.glsl", "../example/shaders/light_shader.fs.glsl");
                })
            );
        scene_root.children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("cube")
                ->add_component<SparrowEngine::Components::Mesh>(SparrowEngine::Example::Constants::cube)
                ->configure_component<SparrowEngine::Components::Mesh>([](auto m) -> void {
                    m->use_shader("../example/shaders/transform_viewspace_shader.vs.glsl", "../example/shaders/object_light_shader.fs.glsl");
//                    m->shader.set_vec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
//                    m->shader.set_vec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
                    m->shader.set_vec3("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
                    m->shader.set_vec3("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
                    m->shader.set_vec3("light.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f));
                    m->shader.set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

                    m->shader.set_vec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
                    m->shader.set_vec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
                    m->shader.set_vec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
                    m->shader.set_float("material.shininess", 32.0f);
                })
                ->add_component<TransformModification>()
            );
        scene_root.children.emplace_back(
            std::make_shared<SparrowEngine::GameObject>("Camera Object")
                ->configure_object([](auto o) -> void {
                    o->transform.position = glm::vec3(1.1f, 1.2f, 3.4f);
                    o->transform.set_euler_angles(-15.0f, 15.0f, 0.0f);
                })
//                ->add_component<TransformModification>()
                ->add_component<SparrowEngine::Components::Camera>()
                ->configure_component<SparrowEngine::Components::Camera>([](auto c) -> void {
                    c->fov = 60.0f;
                })
                ->add_component<CameraMovement>()
                ->configure_component<CameraMovement>([](auto cm) -> void {
                    cm->pitch = -15.0f;
                    cm->yaw = 15.0f;
                })
            );

        SparrowEngine::Scene::start();
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