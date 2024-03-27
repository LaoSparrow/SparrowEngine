#pragma once

#include "sparrow_engine/scene.hpp"

#include "../constants.hpp"
#include "../scripts/camera_movement.hpp"

class Scene3 : public SparrowEngine::Scene {
    void start() override {
        using Mesh = SparrowEngine::Components::Mesh;

#define POINT_LIGHT_POS glm::vec3(0.0f, 0.0f, 4.0f)

        scene_root->add_child_object("Point Light 1");
        scene_root->configure_child_object([](auto obj) {
            obj->transform.position = POINT_LIGHT_POS;
            obj->transform.scale = glm::vec3(0.1f);

            obj->add_component<Mesh>(SparrowEngine::Example::Constants::cube);
            obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                m->use_shader("../example/shaders/standard.vs.glsl", "../example/shaders/object_light_shaderv2.fs.glsl");
                m->shader.load_texture("material.diffuse", "se://texture?color=000000");
                m->shader.load_texture("material.specular", "se://texture?color=000000");
                m->shader.load_texture("material.emission", "se://texture?color=FFFFFF");
                m->shader.set_float("material.shininess", 1.0f);
            });
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

            scene_root->add_child_object(name_buf);
            scene_root->configure_child_object([&cubePositions, &i](auto obj) {
                    obj->transform.position = cubePositions[i];
                    obj->transform.rotation = glm::angleAxis(glm::radians(20.0f * i), glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f)));

                    obj->add_component<Mesh>(SparrowEngine::Example::Constants::cube);
                    obj->configure_component<Mesh>([](std::shared_ptr<Mesh> m) {
                        m->use_shader("../example/shaders/standard.vs.glsl", "../example/shaders/object_light_shaderv2.fs.glsl");
                        m->shader.set_vec3("light.position", POINT_LIGHT_POS);
                        m->shader.set_vec3("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
                        m->shader.set_vec3("light.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f));
                        m->shader.set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

                        m->shader.load_texture("material.diffuse", "../example/textures/container2.png");
                        m->shader.load_texture("material.specular", "../example/textures/container2_specular.png");
                        m->shader.load_texture("material.emission", "se://texture?color=000000");
                        m->shader.set_float("material.shininess", 32.0f);
                    });
                });
        }

        scene_root->add_child_object("Camera Object");
        scene_root->configure_child_object([](auto obj) {
            obj->transform.position = glm::vec3(1.0f, 0.0f, 5.0f);

            obj->add_component<SparrowEngine::Components::Camera>();
            obj->configure_component<SparrowEngine::Components::Camera>([](auto c) {
                c->fov = 60.0f;
            });
            obj->add_component<CameraMovement>();
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