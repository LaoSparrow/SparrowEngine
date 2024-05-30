#pragma once

#include "game_object.hpp"
#include "lighting.hpp"

#include <memory>

namespace SparrowEngine {

    class Scene : public std::enable_shared_from_this<Scene> {
    private:
        bool has_started = false;
        static std::shared_ptr<Scene> current_scene;

    public:
        std::shared_ptr<GameObject> scene_root = std::make_shared<GameObject>("Scene Root");
        std::shared_ptr<Lighting> lighting = std::make_shared<Lighting>();

        void new_frame();

        virtual void start();
        virtual void update();
        virtual void render();

        static std::shared_ptr<Scene> GetCurrentScene();

        friend class Engine;
    };

}