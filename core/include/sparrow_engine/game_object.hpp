#pragma once

#include "behavior.hpp"
#include "transform.hpp"

#include <list>
#include <memory>
#include <string>
#include <functional>

namespace SparrowEngine {

    class GameObject : public std::enable_shared_from_this<GameObject> {
    private:

        void inline start_behaviors() {
            for (auto &s: behaviors) {
                s->start();
            }
        }

        void inline update_behaviors() {
            for (auto &s: behaviors) {
                s->update();
            }
        }

        void inline render_behaviors() {
            for (auto &s: behaviors) {
                s->render();
            }
        }

        void inline start_children() {
            for (auto &c: children) {
                c->start();
            }
        }

        void inline update_children() {
            for (auto &c: children) {
                c->update();
            }
        }

        void inline render_children() {
            for (auto &c: children) {
                c->render();
            }
        }

    public:
        std::string name = "Game Object";
        std::list<std::shared_ptr<GameObject>> children;
        std::list<std::shared_ptr<Behavior>> behaviors;
        Transform transform;

        GameObject() = default;
        explicit GameObject(std::string name);

        virtual void start();

        virtual void update();

        virtual void render();

        template<class BehaviorT, class ...Args>
        std::shared_ptr<GameObject> add_component(Args... args) {
            behaviors.emplace_back(std::make_shared<BehaviorT>(
                weak_from_this(),
                args...));
            return shared_from_this();
        }

        template<class BehaviorT>
        std::shared_ptr<GameObject> configure_component(std::function<void(std::shared_ptr<BehaviorT>)> options) {
            options(std::static_pointer_cast<BehaviorT>(behaviors.back()));
            return shared_from_this();
        }

        std::shared_ptr<GameObject> configure_object(std::function<void(std::shared_ptr<GameObject>)> options);
    };

    GameObject::GameObject(std::string name) : name(name) {

    }

    void GameObject::start() {
        start_behaviors();
        start_children();
    }

    void GameObject::update() {
        update_behaviors();
        update_children();
    }

    void GameObject::render() {
        render_behaviors();
        render_children();
    }

    std::shared_ptr<GameObject> GameObject::configure_object(std::function<void(std::shared_ptr<GameObject>)> options) {
        options(shared_from_this());
        return shared_from_this();
    }

}