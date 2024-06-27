#pragma once

#include "behavior.hpp"
#include "transform.hpp"

#include <list>
#include <memory>
#include <string>
#include <functional>
#include <utility>

namespace SE {

    class GameObject : public std::enable_shared_from_this<GameObject> {
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

        std::shared_ptr<GameObject> configure_child_object(std::function<void(std::shared_ptr<GameObject>)> options);

        std::shared_ptr<GameObject> configure_object(std::function<void(std::shared_ptr<GameObject>)> options);

        glm::mat4 get_model_matrix_in_global();
    };

}