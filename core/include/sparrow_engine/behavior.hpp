#pragma once

#include <memory>

namespace SE {

    class GameObject;

    class Behavior : public std::enable_shared_from_this<Behavior> {
    protected:
        std::weak_ptr<GameObject> game_object;

    public:
        explicit Behavior(std::weak_ptr<GameObject> obj);

        virtual void pre_start();
        virtual void start();
        virtual void post_start();

        virtual void pre_update();
        virtual void update();
        virtual void post_update();

        virtual void pre_render();
        virtual void render();
        virtual void post_render();
    };

}