#pragma once

#include "sparrow_engine/game_object.hpp"
#include "sparrow_engine/scene.hpp"

class BackgroundObject : public SE::GameObject {
public:
    BackgroundObject() : GameObject("background object") {}

    void render() override;
};

class Scene1 : public SE::Scene {
    void start() override;

    void update() override;
};
