#pragma once

#include "behavior.hpp"
#include "transform.hpp"

#include <list>
#include <memory>
#include <string>
#include <functional>
#include <utility>

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
        std::weak_ptr<GameObject> parent;
        std::list<std::shared_ptr<GameObject>> children;
        std::list<std::shared_ptr<Behavior>> behaviors;
        Transform transform;

        GameObject() = default;

        explicit GameObject(std::string name);
        explicit GameObject(std::weak_ptr<GameObject> parent, std::string name);

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

        template<class ...Args>
        std::shared_ptr<GameObject> add_child_object(Args... args) {
            children.emplace_back(std::make_shared<GameObject>(
                weak_from_this(),
                args...
                ));
            return shared_from_this();
        }

        std::shared_ptr<GameObject> configure_child_object(std::function<void(std::shared_ptr<GameObject>)> options) {
            options(std::static_pointer_cast<GameObject>(children.back()));
            return shared_from_this();
        }

        std::shared_ptr<GameObject> configure_object(std::function<void(std::shared_ptr<GameObject>)> options);

        glm::mat4 get_model_matrix_in_global();
    };

    GameObject::GameObject(std::string name) : name(std::move(name)) {

    }

    GameObject::GameObject(std::weak_ptr<GameObject> parent, std::string name)
    : name(std::move(name)),
      parent(std::move(parent)){

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

    glm::mat4 GameObject::get_model_matrix_in_global() {
        glm::mat4 model;
        auto obj = parent.lock();
        if (obj)
            model = obj->get_model_matrix_in_global();
        else
            model = glm::mat4(1.0f);

        model = glm::translate(model, transform.position);
//        model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//        model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//        model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model *= glm::mat4_cast(transform.rotation);
        model = glm::scale(model, transform.scale);

        return model;
    }

}