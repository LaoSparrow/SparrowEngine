#pragma once

#include "sparrow_engine/game_object.hpp"
#include "sparrow_engine/scene.hpp"

class BackgroundObject : public SparrowEngine::GameObject {
public:
    BackgroundObject() : GameObject("background object") {}

    void render() override;
};

class Scene1 : public SparrowEngine::Scene {
    void start() override;

    void update() override;
};
