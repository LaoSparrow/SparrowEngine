#include "kobe.hpp"

#include "sparrow_engine/components/mesh.hpp"
#include "sparrow_engine/components/camera.hpp"
#include "sparrow_engine/components/light.hpp"
#include "sparrow_engine/model_loader.hpp"
#include "sparrow_engine/material.hpp"
#include "sparrow_engine/shader.hpp"
#include "sparrow_engine/texture.hpp"
#include "sparrow_engine/behavior.hpp"

#include "scripts/camera_movement.hpp"
#include "scripts/transform_modification.hpp"
#include "scripts/helicopter_behavior.hpp"
#include "constants.hpp"
#include "common.hpp"

#include "fmt/core.h"

#include <memory>

#ifdef NDEBUG
#define RESOURCES(x) fmt::format("./resources/{}", x)
#else
#define RESOURCES(x) fmt::format("../../../../example/resources/{}", x)
#endif


class RotateAnimation : public SE::Behavior {
public:
    using SE::Behavior::Behavior;

    float delta_radian = 0.01f;
    glm::vec3 rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);

    void update() override {
        auto parent = game_object.lock();
        parent->transform.rotation *= glm::angleAxis(delta_radian, rotate_axis);
        SE::Behavior::update();
    }
};

class Follow : public SE::Behavior {
public:
    using SE::Behavior::Behavior;

    int target_index {};
    std::weak_ptr<SE::GameObject> target;
    bool is_follow_x {};
    bool is_follow_y {};
    bool is_follow_z {};
    bool is_follow_rotation {};

    void start() override {
        auto parent_sp = game_object.lock();
        for (auto const sp : SE::Scene::GetCurrent()->scene_root->children) {
            if (target_index == 0) {
                target = sp->weak_from_this();
                break;
            }
            target_index--;
        }
    }

    void update() override {
        auto parent_sp = game_object.lock();
        auto target_sp = target.lock();
        if (is_follow_x)
            parent_sp->transform.position.x = target_sp->transform.position.x;
        if (is_follow_y)
            parent_sp->transform.position.y = target_sp->transform.position.y;
        if (is_follow_z)
            parent_sp->transform.position.z = target_sp->transform.position.z;
        if (is_follow_rotation)
            parent_sp->transform.rotation = target_sp->transform.rotation;
        SE::Behavior::update();
    }
};

void KobeScene::start() {
    using Mesh = SE::Components::Mesh;
    namespace Scripts = SE::Example::Scripts;

    scene_root->add_child_object("Ground");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.scale = glm::vec3(10000.0f);
        obj->transform.set_euler_angles(90.0f, 0.0f, 0.0f);

        obj->add_component<Mesh>(SE::Example::Constants::plane, SE::Example::Constants::plane_vertex_indices);
        obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
            m->material = SE::Material::create_material(
                SE::Shader::create_shader(RESOURCES("shaders/standard.vs.glsl"), RESOURCES("shaders/standard.fs.glsl")), {
                    { "material.diffuse",   SE::Texture::create_texture(RESOURCES("textures/container.jpg")) },
                    { "material.specular",  SE::Texture::create_texture("se://texture?color=000000") },
                    { "material.emission",  SE::Texture::create_texture("se://texture?color=000000") },
                    { "material.shininess", 1.0f }
                });
        });

//        obj->add_component<Follow>();
//        obj->configure_component<Follow>([](std::shared_ptr<Follow> f) {
//            f->target_index = 2;
//            f->is_follow_x = true;
//            f->is_follow_z = true;
//        });
    });

    scene_root->add_child_object("Directional Light");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.set_euler_angles(100.0f, 10.0f, 0.0f);

        obj->add_component<SE::Components::Light>();
        obj->configure_component<SE::Components::Light>([](std::shared_ptr<SE::Components::Light> l) {
            l->type = SE::Lighting::LightType::DirectionalLight;
            l->ambient = glm::vec3(0.7f, 0.7f, 0.7f);
            l->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            l->specular = glm::vec3(1.0f, 1.0f, 1.0f);
        });

        obj->add_component<Follow>();
        obj->configure_component<Follow>([](std::shared_ptr<Follow> f) {
            f->target_index = 2;
            f->is_follow_x = true;
            f->is_follow_y = true;
            f->is_follow_z = true;
        });

        obj->add_child_object("Sun");
        obj->configure_child_object([](std::shared_ptr<SE::GameObject> indicator) {
            indicator->transform.position = glm::vec3(0.0f, 0.0f, -50.0f);
            indicator->transform.scale = glm::vec3(3.0f);

            indicator->add_component<Mesh>(SE::Example::Constants::cube);
            indicator->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                m->material = SE::Material::create_material(
                    SE::Shader::create_shader(RESOURCES("shaders/standard.vs.glsl"), RESOURCES("shaders/standard.fs.glsl")), {
                        { "material.diffuse",   SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.specular",  SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.emission",  SE::Texture::create_texture("se://texture?color=FFFFFF") },
                        { "material.shininess", 1.0f }
                    });
            });
        });
    });

    scene_root->add_child_object("Helicopter");
    scene_root->configure_child_object([](auto obj) {
        obj->transform.position = glm::vec3(0.0f, 1.2f, 0.0f);
        obj->transform.scale = glm::vec3(0.2f);

//        obj->add_component<Scripts::TransformModification>();

        SE::ModelLoader ml;
        ml.load_model(RESOURCES("models/helicopter/HelicopterBody.obj"));

        for (auto const model : ml.meshes) {
            obj->add_component<Mesh>();
            obj->configure_component<Mesh>([&model](std::shared_ptr<Mesh> m) {
                m->vertices = model.vertices;
                m->vertex_indices = model.indices;
                m->material = SE::Material::create_material(
                    SE::Shader::create_shader(RESOURCES("shaders/standard.vs.glsl"), RESOURCES("shaders/standard.fs.glsl")),
                    {
                        { "material.diffuse", SE::Texture::create_texture(model.texture_paths.diffuse[0]) },
                        { "material.specular", SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.emission", SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.shininess", 32.0f }
                    });
            });
        }

        obj->add_component<Scripts::HelicopterBehavior>();

        obj->add_child_object("Propeller1");
        obj->configure_child_object([](std::shared_ptr<SE::GameObject> obj) {
            obj->transform.position = glm::vec3(0.0f, 5.0f, 27.0f);

            SE::ModelLoader ml;
            ml.load_model(RESOURCES("models/helicopter/HelicopterPropeller1.obj"));
            auto model = ml.meshes[0];
            obj->add_component<Mesh>();
            obj->configure_component<Mesh>([&model](std::shared_ptr<Mesh> m) {
                m->vertices = model.vertices;
                m->vertex_indices = model.indices;
                m->material = SE::Material::create_material(
                    SE::Shader::create_shader(RESOURCES("shaders/standard.vs.glsl"), RESOURCES("shaders/standard.fs.glsl")),
                    {
                        { "material.diffuse", SE::Texture::create_texture(model.texture_paths.diffuse[0]) },
                        { "material.specular", SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.emission", SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.shininess", 32.0f }
                    });
            });

            obj->add_component<RotateAnimation>();
            obj->configure_component<RotateAnimation>([](auto ra) {
                ra->delta_radian = 0.05f;
                ra->rotate_axis = glm::vec3(1.0f, 0.0f, 0.0f);
            });
        });

        obj->add_child_object("Propeller2");
        obj->configure_child_object([](std::shared_ptr<SE::GameObject> obj) {
            obj->transform.position = glm::vec3(0.0f, 5.8f, 3.0f);

            SE::ModelLoader ml;
            ml.load_model(RESOURCES("models/helicopter/HelicopterPropeller2.obj"));
            auto model = ml.meshes[0];
            obj->add_component<Mesh>();
            obj->configure_component<Mesh>([&model](std::shared_ptr<Mesh> m) {
                m->vertices = model.vertices;
                m->vertex_indices = model.indices;
                m->material = SE::Material::create_material(
                    SE::Shader::create_shader(RESOURCES("shaders/standard.vs.glsl"), RESOURCES("shaders/standard.fs.glsl")),
                    {
                        { "material.diffuse", SE::Texture::create_texture(model.texture_paths.diffuse[0]) },
                        { "material.specular", SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.emission", SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.shininess", 32.0f }
                    });
            });

            obj->add_component<RotateAnimation>();
            obj->configure_component<RotateAnimation>([](auto ra) {
                ra->delta_radian = 0.05f;
                ra->rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
            });
        });

//        obj->add_child_object("Camera Object");
//        obj->configure_child_object([](std::shared_ptr<SE::GameObject> obj) {
//            obj->transform.position = glm::vec3(0.0f, 17.5f, 29.0f);
//            obj->transform.set_euler_angles(-11.0f, 0.0f, 0.0f);
//
////            obj->add_component<Scripts::CameraMovement>();
////            obj->add_component<Scripts::TransformModification>();
//
//            obj->add_component<SE::Components::Camera>();
//            obj->configure_component<SE::Components::Camera>([](auto c) {
//                c->fov = 80.0f;
//            });
//        });
    });

    scene_root->add_child_object("Camera Pivot");
    scene_root->configure_child_object([](std::shared_ptr<SE::GameObject> obj) {
        obj->transform.set_euler_angles(-11.0f, 0.0f, 0.0f);

        obj->add_component<Scripts::CameraMovement>(); // BAAAAAD! very bad! but i'm lazy
        obj->configure_component<Scripts::CameraMovement>([](std::shared_ptr<Scripts::CameraMovement> cm) {
            cm->pitch = -11.0f;
        });
        obj->add_component<Follow>();
        obj->configure_component<Follow>([](std::shared_ptr<Follow> f) {
            f->target_index = 2;
            f->is_follow_x = true;
            f->is_follow_y = true;
            f->is_follow_z = true;
        });

        obj->add_child_object("Camera Object");
        obj->configure_child_object([](std::shared_ptr<SE::GameObject> obj) {
            obj->transform.position = glm::vec3(0.0f, 0.0f, glm::length(glm::vec3(0.0f, 17.5f, 29.0f) * 0.4f));

            obj->add_component<SE::Components::Camera>();
            obj->configure_component<SE::Components::Camera>([](auto c) {
                c->fov = 80.0f;
            });
        });
    });

    glm::vec3 kobe_positions[] = {
        glm::vec3(500.0f, 300.0f, 500.0f),
        glm::vec3(-500.0f, 400.0f, 200.0f),
        glm::vec3(700.0f, 100.0f, -500.0f),
        glm::vec3(400.0f, 250.0f, -700.0f),
        glm::vec3(-700.0f, 50.0f, 100.0f),
        glm::vec3(-600.0f, 300.0f, 400.0f)
    };
    for (int i = 0; i < sizeof(kobe_positions)/sizeof(kobe_positions[0]); i++) {
        scene_root->add_child_object(fmt::format("Kobe {}", i));
        scene_root->configure_child_object([&kobe_positions, i](auto obj) {
            obj->transform.position = kobe_positions[i];
            obj->transform.scale = glm::vec3(100.0f);
            obj->transform.rotation = glm::angleAxis(0.1f * i, glm::vec3(0.0f, 1.0f, 0.0f));

            obj->add_component<Mesh>(SE::Example::Constants::plane, SE::Example::Constants::plane_vertex_indices);
            obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                m->material = SE::Material::create_material(
                    SE::Shader::create_shader(RESOURCES("shaders/standard.vs.glsl"), RESOURCES("shaders/standard.fs.glsl")), {
                        { "material.diffuse",   SE::Texture::create_texture(RESOURCES("textures/kobe.png")) },
                        { "material.specular",  SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.emission",  SE::Texture::create_texture("se://texture?color=000000") },
                        { "material.shininess", 1.0f }
                    });
            });

            obj->add_component<RotateAnimation>();
        });
    }
    Scene::start();
}

void KobeScene::update() {
    SE::Example::Scenes::Common::ProcessKeys();
    Scene::update();
}

void KobeScene::render() {
    glClearColor(188.0f/256.0f, 226.0f/256.0f, 238.0f/256.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Scene::render();
}
