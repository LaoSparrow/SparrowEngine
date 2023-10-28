#pragma once

#include "game_object.hpp"

#include <memory>

namespace SparrowEngine {

    class Scene : public std::enable_shared_from_this<Scene> {
    private:
        bool is_started = false;

    public:
        GameObject scene_root;

        void update();
        void render();
    };

}