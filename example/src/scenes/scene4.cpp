#include "scene4.hpp"
#include "common.hpp"

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

namespace Components = SE::Components;
namespace Scripts = SE::Example::Scripts;

using GameObject = SE::GameObject;
using Material = SE::Material;
using Shader = SE::Shader;
using Constants = SE::Example::Constants;

void Scene4::start() {
    using Mesh = SE::Components::Mesh;

    scene_root->add_child_object("Directional Light");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.set_euler_angles(100.0f, 10.0f, 0.0f);

        obj->add_component<SE::Components::Light>();
        obj->configure_component<SE::Components::Light>([](std::shared_ptr<SE::Components::Light> l) {
            l->type = SE::Lighting::LightType::DirectionalLight;
            l->ambient = glm::vec3(0.3f, 0.3f, 0.3f);
            l->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            l->specular = glm::vec3(1.0f, 1.0f, 1.0f);
        });

        obj->add_child_object("Sun");
        obj->configure_child_object([](std::shared_ptr<SE::GameObject> indicator) {
            indicator->transform.position = glm::vec3(0.0f, 0.0f, -50.0f);
            indicator->transform.scale = glm::vec3(3.0f);

            indicator->add_component<Mesh>(SE::Example::Constants::cube);
            indicator->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                m->material = SE::Material::create_material(
                    SE::Shader::create_shader("../../../../example/resources/shaders/standard.vs.glsl", "../../../../example/resources/shaders/standard.fs.glsl"), {
                        { "material.diffuse",   SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.specular",  SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.emission",  SE::Texture::create_texture("se://texture?color=FFFFFF") },
                        { "material.shininess", 1.0f }
                    });
            });
        });
    });

    scene_root->add_child_object("Backpack");
    scene_root->configure_child_object([](auto obj) {
        SE::ModelLoader loader;
        loader.load_model("../../../../example/resources/models/backpack/backpack.obj", aiProcess_FlipUVs);
        for (const auto &m : loader.meshes) {
            obj->add_component<Mesh>();
            obj->configure_component<Mesh>([&m](std::shared_ptr<Mesh> mc) {
                mc->vertices = m.vertices;
                mc->vertex_indices = m.indices;
                mc->material = SE::Material::create_material(
                    SE::Shader::create_shader("../../../../example/resources/shaders/standard.vs.glsl", "../../../../example/resources/shaders/standard.fs.glsl"), {
                        { "material.diffuse",   SE::Texture::create_texture(m.texture_paths.diffuse[0]) },
                        { "material.specular",  SE::Texture::create_texture(m.texture_paths.specular[0]) },
                        { "material.emission",  SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.shininess", 1.0f }
                    });
            });
        }

        obj->add_component<Scripts::TransformModification>();
    });

    scene_root->add_child_object("Camera Object");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.position = glm::vec3(1.0f, 0.0f, 5.0f);

        obj->add_component<Scripts::CameraMovement>();

        obj->add_component<SE::Components::Camera>();
        obj->configure_component<SE::Components::Camera>([](auto c) {
            c->fov = 60.0f;
        });

//            obj->add_child_object("Spot Light 1");
//            obj->configure_child_object([](std::shared_ptr<SE::GameObject> light_obj) {
//                light_obj->transform.position = glm::vec3(0.4f, -0.2f, -0.5f);
//                light_obj->transform.set_euler_angles(0.0f, 180.0f, 0.0f);
//                light_obj->transform.scale = glm::vec3(0.1f);
//
//                light_obj->add_component<Mesh>(SE::Example::Constants::cube);
//                light_obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
//                    m->material = SE::Material::create_material(
//                        SE::Shader::create_shader("../../../../example/resources/shaders/standard.vs.glsl", "../../../../example/resources/shaders/standard.fs.glsl"), {
//                            { "material.diffuse", SE::Texture::create_texture("se://texture?color=000000") },
//                            { "material.specular", SE::Texture::create_texture("se://texture?color=000000") },
//                            { "material.emission", SE::Texture::create_texture("se://texture?color=FFFFFF") },
//                            { "material.shininess", 1.0f }
//                        });
//                });
//
//                light_obj->add_component<SE::Components::Light>();
//                light_obj->configure_component<SE::Components::Light>([](std::shared_ptr<SE::Components::Light> l) {
//                    l->type = SE::Lighting::LightType::SpotLight;
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

    SE::Scene::start();
}

void Scene4::update() {
    SE::Example::Scenes::Common::ProcessKeys();
    SE::Scene::update();
}


