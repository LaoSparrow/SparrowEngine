#include "sparrow_engine/scene.hpp"

using namespace SparrowEngine;

void Scene::new_frame() {
    if (!has_started) {
        has_started = true;
        lighting->initialize();
        start();
    }
    lighting->use();
    update();
}

void Scene::start() {
    scene_root->start();
}

void Scene::update() {
    scene_root->update();
}

void Scene::render() {
    scene_root->render();
}

std::shared_ptr<Scene> Scene::current_scene;

std::shared_ptr<Scene> Scene::GetCurrentScene() {
    return current_scene;
}