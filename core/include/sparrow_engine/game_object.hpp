#pragma once

#include "behavior.hpp"

#include <list>
#include <memory>
#include <string>

namespace SparrowEngine {

    class GameObject : public std::enable_shared_from_this<GameObject> {
    private:

        void inline start_behaviors() {
            for (auto &s: scripts) {
                s->start();
            }
        }

        void inline update_behaviors() {
            for (auto &s: scripts) {
                s->update();
            }
        }

        void inline render_behaviors() {
            for (auto &s: scripts) {
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
        std::string name;
        std::list<std::shared_ptr<GameObject>> children;
        std::list<std::shared_ptr<Behavior>> scripts;

        GameObject() = default;
        explicit GameObject(std::string name);

        virtual void start();

        virtual void update();

        virtual void render();

        template<class BehaviorT, class ...Args>
        std::shared_ptr<GameObject> add_component(Args... args) {
            scripts.emplace_back(std::make_shared<BehaviorT>(
                std::weak_ptr<GameObject>(shared_from_this()),
                args...));
            return shared_from_this();
        }
    };

}