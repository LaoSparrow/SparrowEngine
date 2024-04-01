#pragma once

#include "sparrow_engine/time.hpp"
#include "sparrow_engine/scene.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/shader.hpp"
#include "sparrow_engine/material.hpp"
#include "sparrow_engine/model_loader.hpp"
#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/components/mesh.hpp"
#include "sparrow_engine/components/light.hpp"
#include "sparrow_engine/components/camera.hpp"

#include "assimp/postprocess.h"
#include "glm/gtc/quaternion.hpp"

#include "../constants.hpp"
#include "../scripts/transform_modification.hpp"
#include "../scripts/camera_movement.hpp"

#include <memory>

class Spin : public SparrowEngine::Behavior {
public:
    using SparrowEngine::Behavior::Behavior;

    void update() override {
        auto parent_obj = game_object.lock();
        parent_obj->transform.rotation *= glm::angleAxis(glm::radians(20.0f * (float)SparrowEngine::Time::GetDeltaTime()), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};

class UpDownMovement : public SparrowEngine::Behavior {
public:
    using SparrowEngine::Behavior::Behavior;

    void update() override {
        auto parent_obj = game_object.lock();
        parent_obj->transform.position.y += -sin((float)SparrowEngine::Time::GetTimeNow()) * 0.01f;
    }
};

class VelociraptorScene : public SparrowEngine::Scene {
    void start() override {
        using Mesh = SparrowEngine::Components::Mesh;
        using Material = SparrowEngine::Material;
        using Shader = SparrowEngine::Shader;

        scene_root->add_child_object("Directional Light");
        scene_root->configure_child_object([](auto obj) {
            obj->transform.set_euler_angles(100.0f, 10.0f, 0.0f);

            obj->add_component<SparrowEngine::Components::Light>();
            obj->configure_component<SparrowEngine::Components::Light>([](std::shared_ptr<SparrowEngine::Components::Light> l) {
                l->type = SparrowEngine::Lighting::LightType::DirectionalLight;
                l->ambient = glm::vec3(0.3f, 0.3f, 0.3f);
                l->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
                l->specular = glm::vec3(1.0f, 1.0f, 1.0f);
            });

            obj->add_child_object("Sun");
            obj->configure_child_object([](std::shared_ptr<SparrowEngine::GameObject> indicator) {
                indicator->transform.position = glm::vec3(0.0f, 0.0f, -50.0f);
                indicator->transform.scale = glm::vec3(3.0f);

                indicator->add_component<Mesh>(SparrowEngine::Example::Constants::cube);
                indicator->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                    m->material = SparrowEngine::Material::create_material(
                        SparrowEngine::Shader::create_shader("../example/shaders/standard.vs.glsl", "../example/shaders/standard.fs.glsl"), {
                            { "material.diffuse", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.specular", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.emission", SparrowEngine::Texture::create_texture("se://texture?color=FFFFFF") },
                            { "material.shininess", 1.0f }
                        });
                });
            });
        });

        scene_root->add_child_object("Object");
        scene_root->configure_child_object([](auto obj) {
            obj->add_component<Spin>();

            obj->add_child_object("Velociraptor");
            obj->configure_child_object([](auto velociraptor) {
                velociraptor->transform.scale = glm::vec3(0.02f);

                SparrowEngine::ModelLoader loader;
                loader.load_model("../example/models/tropical-velociraptor/Velociraptor pose V1.fbx");
                velociraptor->add_component<Mesh>();
                velociraptor->configure_component<Mesh>([&loader](std::shared_ptr<Mesh> mc) {
                    mc->vertices = loader.meshes[0].vertices;
                    mc->vertex_indices = loader.meshes[0].indices;
                    mc->material = SparrowEngine::Material::create_material(
                        SparrowEngine::Shader::create_shader("../example/shaders/standard.vs.glsl", "../example/shaders/standard.fs.glsl"), {
                            { "material.diffuse", SparrowEngine::Texture::create_texture("../example/models/tropical-velociraptor/Raptor Color 2.png") },
                            { "material.specular", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.emission", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.shininess", 1.0f }
                        });
                });

//            velociraptor->add_component<TransformModification>();
            });

            obj->add_child_object("Egg");
            obj->configure_child_object([](auto egg) {
                egg->transform.position = glm::vec3(0.46f, 1.5f, -0.23f);
                egg->transform.scale = glm::vec3(0.1f);

                egg->add_component<UpDownMovement>();

                SparrowEngine::ModelLoader loader;
                loader.load_model("../example/models/easter-eggs/Easter_Eggs_Low_Poly.obj");
                egg->add_component<Mesh>();
                egg->configure_component<Mesh>([&loader](std::shared_ptr<Mesh> mc) {
                    mc->vertices = loader.meshes[0].vertices;
                    mc->vertex_indices = loader.meshes[0].indices;
                    mc->material = SparrowEngine::Material::create_material(
                        SparrowEngine::Shader::create_shader("../example/shaders/standard.vs.glsl", "../example/shaders/standard.fs.glsl"), {
                            { "material.diffuse", SparrowEngine::Texture::create_texture("../example/models/easter-eggs/EGG_TEX_3.jpg") },
                            { "material.specular", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.emission", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.shininess", 1.0f }
                        });
                });

//                egg->add_component<TransformModification>();
            });

            obj->add_child_object("Avatar");
            obj->configure_child_object([](std::shared_ptr<SparrowEngine::GameObject> avatar) {
                avatar->transform.position = glm::vec3(-0.05f, 2.23f, 2.0f);
                avatar->transform.set_euler_angles(90.0f, 0.0f, 0.0f);

                avatar->add_component<Mesh>(SparrowEngine::Example::Constants::cube);
                avatar->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                    m->material = SparrowEngine::Material::create_material(
                        SparrowEngine::Shader::create_shader("../example/shaders/standard.vs.glsl", "../example/shaders/standard.fs.glsl"), {
                            { "material.diffuse", SparrowEngine::Texture::create_texture("../example/textures/avatar.png") },
                            { "material.specular", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.emission", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.shininess", 1.0f }
                        });
                });

//                avatar->add_component<TransformModification>();
            });
        });

        scene_root->add_child_object("Camera Object");
        scene_root->configure_child_object([](auto obj) {
            obj->transform.position = glm::vec3(1.0f, 0.0f, 5.0f);

            obj->add_component<CameraMovement>();

            obj->add_component<SparrowEngine::Components::Camera>();
            obj->configure_component<SparrowEngine::Components::Camera>([](auto c) {
                c->fov = 60.0f;
            });
        });

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