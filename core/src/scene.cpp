#include "sparrow_engine/scene.hpp"

#include <ranges>

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
    obj_stack.push(scene_root);
    while (!obj_stack.empty()) {
        auto obj = obj_stack.top();
        obj_stack.pop();
        obj->start();
        for (const auto& b : obj->behaviors) {
            b->start();
        }
        for (const auto& child_obj : obj->children | std::views::reverse) {
            obj_stack.push(child_obj);
        }
    }
}

void Scene::update() {
    obj_stack.push(scene_root);
    while (!obj_stack.empty()) {
        auto obj = obj_stack.top();
        obj_stack.pop();
        obj->update();
        for (const auto& b : obj->behaviors) {
            b->update();
        }
        for (const auto& child_obj : obj->children | std::views::reverse) {
            obj_stack.push(child_obj);
        }
    }
}

void Scene::render() {
    obj_stack.push(scene_root);
    while (!obj_stack.empty()) {
        auto obj = obj_stack.top();
        obj_stack.pop();
        obj->render();
        for (const auto& b : obj->behaviors) {
            b->render();
        }
        for (const auto& child_obj : obj->children | std::views::reverse) {
            obj_stack.push(child_obj);
        }
    }
}

std::shared_ptr<Scene> Scene::current_scene;

std::shared_ptr<Scene> Scene::GetCurrentScene() {
    return current_scene;
}