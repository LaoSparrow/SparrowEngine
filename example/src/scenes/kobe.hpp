#pragma once

#include "sparrow_engine/scene.hpp"

class KobeScene : public SE::Scene {
public:
    void start() override;
    void update() override;
    void render() override;
};
