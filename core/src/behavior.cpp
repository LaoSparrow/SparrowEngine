#include "sparrow_engine/behavior.hpp"

using namespace SparrowEngine;

Behavior::Behavior(std::weak_ptr<GameObject> obj) {
    game_object = std::move(obj);
}

void Behavior::start() {

}

void Behavior::update() {

}

void Behavior::render() {

}