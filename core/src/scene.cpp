#include "sparrow_engine/scene.hpp"

namespace SparrowEngine {

    void Scene::update() {
        if (!is_started) {
            is_started = true;
            scene_root.start();
        }

        scene_root.update();
    }

    void Scene::render() {
        scene_root.render();
    }

}