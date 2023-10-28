#include "sparrow_engine/game_object.hpp"

namespace SparrowEngine {

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

}