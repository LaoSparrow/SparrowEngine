#include "sparrow_engine/behavior.hpp"

using namespace SE;

Behavior::Behavior(std::weak_ptr<GameObject> obj) {
    game_object = std::move(obj);
}

void Behavior::pre_start() {}
void Behavior::start() {}
void Behavior::post_start() {}

void Behavior::pre_update() {}
void Behavior::update() {}
void Behavior::post_update() {}

void Behavior::pre_render() {}
void Behavior::render() {}
void Behavior::post_render() {}