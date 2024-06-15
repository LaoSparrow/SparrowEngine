#include "scene1.hpp"

#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/time.hpp"
#include "sparrow_engine/components/mesh.hpp"
#include "sparrow_engine/components/camera.hpp"
#include "sparrow_engine/material.hpp"

#include "glm/gtc/quaternion.hpp"

#include "../scripts/transform_modification.hpp"
#include "../scripts/camera_movement.hpp"
#include "../constants.hpp"

namespace Components = SparrowEngine::Components;
namespace Scripts = SparrowEngine::Example::Scripts;

using GameObject = SparrowEngine::GameObject;
using Material = SparrowEngine::Material;
using Shader = SparrowEngine::Shader;
using Constants = SparrowEngine::Example::Constants;
using Texture = SparrowEngine::Texture;

void BackgroundObject::render() {
    double now = SparrowEngine::Time::GetTimeNow();
    glClearColor(sin(now) * 0.5f + 0.5f, cos(now) * 0.5f + 0.5f, sin(now * 2) * 0.5f + 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    GameObject::render();
}

void Scene1::start() {
    scene_root->children.emplace_back(std::make_shared<BackgroundObject>());
    using Mesh = Components::Mesh;
    scene_root->children.emplace_back(
        std::make_shared<GameObject>("Plane")
            ->add_component<Scripts::TransformModification>()
            ->add_component<Components::Mesh>(
                Constants::plane,
                Constants::plane_vertex_indices)
            ->configure_component<Components::Mesh>([](auto m) {
                m->material = Material::create_material(
                    Shader::create_shader("../../../../example/resources/shaders/transform_shader.vs.glsl", "../../../../example/resources/shaders/double_texture_shader.fs.glsl"), {
                        { "texture_0", Texture::create_texture("../../../../example/resources/textures/container.jpg") },
                        { "texture_1", Texture::create_texture("../../../../example/resources/textures/awesomeface.png") }
                    });
            })
    );
    scene_root->children.emplace_back(
        std::make_shared<GameObject>("Cube")
            ->add_component<Scripts::TransformModification>()
            ->add_component<Components::Mesh>(
                Constants::cube)
            ->configure_component<Components::Mesh>([](auto m) {
                m->material = Material::create_material(
                    Shader::create_shader("../../../../example/resources/shaders/transform_shader.vs.glsl", "../../../../example/resources/shaders/double_texture_shader.fs.glsl"), {
                        { "texture_0", Texture::create_texture("../../../../example/resources/textures/container.jpg") },
                        { "texture_1", Texture::create_texture("../../../../example/resources/textures/awesomeface.png") }
                    });
            })

            ->add_child_object("Cube child")
            ->configure_child_object([](auto obj) {
                obj->transform.position = glm::vec3(1.0f, 1.0f, 0.0f);
                obj->transform.rotation *= glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                obj->add_component<Scripts::TransformModification>();
                obj->add_component<Mesh>(
                    Constants::cube);
                obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                    m->material = Material::create_material(
                        Shader::create_shader("../../../../example/resources/shaders/transform_shader.vs.glsl", "../../../../example/resources/shaders/double_texture_shader.fs.glsl"), {
                            { "texture_0", Texture::create_texture("../../../../example/resources/textures/container.jpg") },
                            { "texture_1", Texture::create_texture("../../../../example/resources/textures/awesomeface.png") }
                        });
                });
            })
    );
    scene_root->children.emplace_back(
        std::make_shared<GameObject>("Camera Object")
            ->configure_object([](auto o) {
                o->transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
            })
            ->add_component<Scripts::TransformModification>()
            ->add_component<Components::Camera>()
            ->add_component<Scripts::CameraMovement>()
    );
//        scene_root.children.emplace_back(
//            std::make_shared<GameObject>("Camera Parent")
//                ->add_component<Scripts::TransformModification>()
//
//                ->add_child_object("Camera Object")
//                ->configure_child_object([](auto cam) -> void {
//                    cam->transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
//                    cam->add_component<Components::Camera>();
//                    cam->add_component<CameraMovement>();
//                })
//        );
    Scene::start();
}

void Scene1::update() {
    GLFWwindow *w = SparrowEngine::GameWindow::GetCurrentActiveWindow()->glfw_window;
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "Closing current window..." << std::endl;
        glfwSetWindowShouldClose(w, true);
    }
    Scene::update();
}

