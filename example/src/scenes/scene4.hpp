#pragma once

#include "sparrow_engine/scene.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/shader.hpp"
#include "sparrow_engine/material.hpp"
#include "sparrow_engine/model_loader.hpp"
#include "sparrow_engine/components/mesh.hpp"
#include "sparrow_engine/components/light.hpp"
#include "sparrow_engine/components/camera.hpp"

#include "assimp/postprocess.h"

#include "../constants.hpp"
#include "../scripts/transform_modification.hpp"
#include "../scripts/camera_movement.hpp"

#include <memory>

class Scene4 : public SparrowEngine::Scene {
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

        scene_root->add_child_object("Backpack");
        scene_root->configure_child_object([](auto obj) {
            SparrowEngine::ModelLoader loader;
            loader.load_model("../example/models/backpack/backpack.obj", aiProcess_FlipUVs);
            for (const auto &m : loader.meshes) {
                obj->add_component<Mesh>();
                obj->configure_component<Mesh>([&m](std::shared_ptr<Mesh> mc) {
                    mc->vertices = m.vertices;
                    mc->vertex_indices = m.indices;
                    mc->material = SparrowEngine::Material::create_material(
                        SparrowEngine::Shader::create_shader("../example/shaders/standard.vs.glsl", "../example/shaders/standard.fs.glsl"), {
                            { "material.diffuse", SparrowEngine::Texture::create_texture(m.texture_paths.diffuse[0]) },
                            { "material.specular", SparrowEngine::Texture::create_texture(m.texture_paths.specular[0]) },
                            { "material.emission", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
                            { "material.shininess", 1.0f }
                        });
                });
            }

            obj->add_component<TransformModification>();
        });

        scene_root->add_child_object("Camera Object");
        scene_root->configure_child_object([](auto obj) {
            obj->transform.position = glm::vec3(1.0f, 0.0f, 5.0f);

            obj->add_component<CameraMovement>();

            obj->add_component<SparrowEngine::Components::Camera>();
            obj->configure_component<SparrowEngine::Components::Camera>([](auto c) {
                c->fov = 60.0f;
            });

//            obj->add_child_object("Spot Light 1");
//            obj->configure_child_object([](std::shared_ptr<SparrowEngine::GameObject> light_obj) {
//                light_obj->transform.position = glm::vec3(0.4f, -0.2f, -0.5f);
//                light_obj->transform.set_euler_angles(0.0f, 180.0f, 0.0f);
//                light_obj->transform.scale = glm::vec3(0.1f);
//
//                light_obj->add_component<Mesh>(SparrowEngine::Example::Constants::cube);
//                light_obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
//                    m->material = SparrowEngine::Material::create_material(
//                        SparrowEngine::Shader::create_shader("../example/shaders/standard.vs.glsl", "../example/shaders/standard.fs.glsl"), {
//                            { "material.diffuse", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
//                            { "material.specular", SparrowEngine::Texture::create_texture("se://texture?color=000000") },
//                            { "material.emission", SparrowEngine::Texture::create_texture("se://texture?color=FFFFFF") },
//                            { "material.shininess", 1.0f }
//                        });
//                });
//
//                light_obj->add_component<SparrowEngine::Components::Light>();
//                light_obj->configure_component<SparrowEngine::Components::Light>([](std::shared_ptr<SparrowEngine::Components::Light> l) {
//                    l->type = SparrowEngine::Lighting::LightType::SpotLight;
////                    l->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
//                    l->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
//                    l->specular = glm::vec3(1.0f, 1.0f, 1.0f);
//                    l->constant = 1.0f;
//                    l->linear = 0.014f;
//                    l->quadratic = 0.0007f;
//                    l->cut_off = glm::cos(glm::radians(12.5f));
//                    l->outer_cut_off = glm::cos(glm::radians(17.5f));
//                });
//            });
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