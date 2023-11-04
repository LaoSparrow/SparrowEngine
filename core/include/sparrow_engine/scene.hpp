#pragma once

#include "game_object.hpp"

#include <memory>

namespace SparrowEngine {

    class Scene : public std::enable_shared_from_this<Scene> {
    private:
        bool is_started = false;

    public:
        GameObject scene_root;

        void new_frame();

        virtual void start();
        virtual void update();
        virtual void render();
    };

    void Scene::new_frame() {
        if (!is_started) {
            is_started = true;
            start();
        }
        update();
    }

    void Scene::start() {
        scene_root.start();
    }

    void Scene::update() {
        scene_root.update();
    }

    void Scene::render() {
        scene_root.render();
    }

}