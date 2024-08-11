#pragma once

#include "sparrow_engine/behavior.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

namespace SE::Example::Scripts {
    class HelicopterBehavior : public SE::Behavior {
    public:
        using SE::Behavior::Behavior;

        float ground_y = 1.2f;
        float ground_friction = 5000.0f;
        float gravitational_constant = -98.1f;
        float mass = 100.0f;
        float throttle = 0.0f;
        float auto_correction = 1.5f;
        float air_resistance = 0.2f;
        glm::vec3 velocity {};
        glm::vec3 force {};
        glm::vec3 external_force {};

        void update() override;
    };
}

