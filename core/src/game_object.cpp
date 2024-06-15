#include "sparrow_engine/game_object.hpp"

using namespace SparrowEngine;

GameObject::GameObject(std::string name) : name(std::move(name)) {

}

GameObject::GameObject(std::weak_ptr<GameObject> parent, std::string name)
    : name(std::move(name)),
      parent(std::move(parent)){

}

void GameObject::start() {

}

void GameObject::update() {

}

void GameObject::render() {

}

std::shared_ptr<GameObject> GameObject::configure_child_object(std::function<void(std::shared_ptr<GameObject>)> options) {
    options(std::static_pointer_cast<GameObject>(children.back()));
    return shared_from_this();
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