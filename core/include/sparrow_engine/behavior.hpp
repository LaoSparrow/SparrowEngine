#pragma once

#include <memory>

namespace SparrowEngine {

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

    Behavior::Behavior(std::weak_ptr<GameObject> obj) {
        game_object = std::move(obj);
    }

    void Behavior::start() {

    }

    void Behavior::update() {

    }

    void Behavior::render() {

    }

}