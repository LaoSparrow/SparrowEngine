#include "scene3.hpp"
#include "common.hpp"

#include "sparrow_engine/components/camera.hpp"
#include "sparrow_engine/components/light.hpp"
#include "sparrow_engine/shader.hpp"
#include "sparrow_engine/material.hpp"

#include "../constants.hpp"
#include "../scripts/camera_movement.hpp"
#include "../scripts/transform_modification.hpp"

namespace Components = SE::Components;
namespace Scripts = SE::Example::Scripts;

using GameObject = SE::GameObject;
using Material = SE::Material;
using Shader = SE::Shader;
using Constants = SE::Example::Constants;

void Scene3::start() {
    using Mesh = SE::Components::Mesh;

    scene_root->add_child_object("Directional Light");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.set_euler_angles(100.0f, 10.0f, 0.0f);

        obj->add_component<SE::Components::Light>();
        obj->configure_component<SE::Components::Light>([](std::shared_ptr<SE::Components::Light> l) {
            l->type = SE::Lighting::LightType::DirectionalLight;
//                l->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
            l->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
            l->specular = glm::vec3(1.0f, 1.0f, 1.0f);
        });

        obj->add_component<Scripts::TransformModification>();

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

    scene_root->add_child_object("Point Light 1");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.position = glm::vec3(0.0f, 0.0f, 4.0f);
        obj->transform.scale = glm::vec3(0.1f);

        obj->add_component<Mesh>(SE::Example::Constants::cube);
        obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
            m->material = SE::Material::create_material(
                SE::Shader::create_shader("../../../../example/resources/shaders/standard.vs.glsl", "../../../../example/resources/shaders/standard.fs.glsl"), {
                    { "material.diffuse",   SE::Texture::create_texture("se://texture?color=000000") },
                    { "material.specular",  SE::Texture::create_texture("se://texture?color=000000") },
                    { "material.emission",  SE::Texture::create_texture("se://texture?color=FFFFFF") },
                    { "material.shininess", 1.0f }
                });
        });

        obj->add_component<SE::Components::Light>();
        obj->configure_component<SE::Components::Light>([](std::shared_ptr<SE::Components::Light> l) {
            l->type = SE::Lighting::LightType::PointLight;
//                l->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
            l->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
            l->specular = glm::vec3(1.0f, 1.0f, 1.0f);
            l->constant = 1.0f;
            l->linear = 0.09f;
            l->quadratic = 0.032f;
        });

        obj->add_component<Scripts::TransformModification>();
    });

    scene_root->add_child_object("Point Light 2");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.position = glm::vec3(0.0f, 2.0f, -4.0f);
        obj->transform.scale = glm::vec3(0.1f);

        obj->add_component<Mesh>(SE::Example::Constants::cube);
        obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
            m->material = SE::Material::create_material(
                SE::Shader::create_shader("../../../../example/resources/shaders/standard.vs.glsl", "../../../../example/resources/shaders/standard.fs.glsl"), {
                    { "material.diffuse",   SE::Texture::create_texture("se://texture?color=000000") },
                    { "material.specular",  SE::Texture::create_texture("se://texture?color=000000") },
                    { "material.emission",  SE::Texture::create_texture("se://texture?color=FF0000") },
                    { "material.shininess", 1.0f }
                });
        });

        obj->add_component<SE::Components::Light>();
        obj->configure_component<SE::Components::Light>([](std::shared_ptr<SE::Components::Light> l) {
            l->type = SE::Lighting::LightType::PointLight;
//                l->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
            l->diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
            l->specular = glm::vec3(1.0f, 0.0f, 0.0f);
            l->constant = 1.0f;
            l->linear = 0.09f;
            l->quadratic = 0.032f;
        });

        obj->add_component<Scripts::TransformModification>();
    });

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    for (int i = 0; i < 10; i++) {
        char name_buf[10];
        sprintf(name_buf, "Cube %d", i);

        auto cube_material = SE::Material::create_material(
            SE::Shader::create_shader("../../../../example/resources/shaders/standard.vs.glsl", "../../../../example/resources/shaders/standard.fs.glsl"), {
                { "material.diffuse",   SE::Texture::create_texture("../../../../example/resources/textures/container2.png") },
                { "material.specular",  SE::Texture::create_texture("../../../../example/resources/textures/container2_specular.png") },
                { "material.emission",  SE::Texture::create_texture("se://texture?color=000000") },
                { "material.shininess", 32.0f }
            });

        scene_root->add_child_object(name_buf);
        scene_root->configure_child_object([&cubePositions, &i, cube_material](auto obj) {
            obj->transform.position = cubePositions[i];
            obj->transform.rotation = glm::angleAxis(glm::radians(20.0f * i), glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f)));

            obj->add_component<Mesh>(SE::Example::Constants::cube);
            obj->configure_component<Mesh>([cube_material](std::shared_ptr<Mesh> m) {
                m->material = cube_material;
            });
        });
    }

    scene_root->add_child_object("Camera Object");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.position = glm::vec3(1.0f, 0.0f, 5.0f);

        obj->add_component<Scripts::CameraMovement>();

        obj->add_component<SE::Components::Camera>();
        obj->configure_component<SE::Components::Camera>([](auto c) {
            c->fov = 60.0f;
        });

        obj->add_child_object("Spot Light 1");
        obj->configure_child_object([](std::shared_ptr<SE::GameObject> light_obj) {
            light_obj->transform.position = glm::vec3(0.4f, -0.2f, -0.5f);
            light_obj->transform.set_euler_angles(0.0f, 180.0f, 0.0f);
            light_obj->transform.scale = glm::vec3(0.1f);

            light_obj->add_component<Mesh>(SE::Example::Constants::cube);
            light_obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                m->material = SE::Material::create_material(
                    SE::Shader::create_shader("../../../../example/resources/shaders/standard.vs.glsl", "../../../../example/resources/shaders/standard.fs.glsl"), {
                        { "material.diffuse",   SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.specular",  SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.emission",  SE::Texture::create_texture("se://texture?color=FFFFFF") },
                        { "material.shininess", 1.0f }
                    });
            });

            light_obj->add_component<SE::Components::Light>();
            light_obj->configure_component<SE::Components::Light>([](std::shared_ptr<SE::Components::Light> l) {
                l->type = SE::Lighting::LightType::SpotLight;
//                    l->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
                l->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
                l->specular = glm::vec3(1.0f, 1.0f, 1.0f);
                l->constant = 1.0f;
                l->linear = 0.014f;
                l->quadratic = 0.0007f;
                l->cut_off = glm::cos(glm::radians(12.5f));
                l->outer_cut_off = glm::cos(glm::radians(17.5f));
            });
        });
    });

    SE::Scene::start();
}

void Scene3::update() {
    SE::Example::Scenes::Common::ProcessKeys();
    SE::Scene::update();
}


