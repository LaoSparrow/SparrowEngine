#pragma once

#include <memory>

namespace SE {

    class GameObject;

    class Behavior : public std::enable_shared_from_this<Behavior> {
    protected:
        std::weak_ptr<GameObject> game_object;

    public:
        explicit Behavior(std::weak_ptr<GameObject> obj);

        virtual void start();
        virtual void update();
        virtual void render();
    };

}