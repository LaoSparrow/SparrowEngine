#pragma once

#include "sparrow_engine/behavior.hpp"

#include "glm/gtc/quaternion.hpp"
#include "glm/vec3.hpp"

namespace SE::Example::Scripts {
    class TransformModification : public SE::Behavior {
    private:
        glm::vec3 euler_angle;
        glm::quat orig_quat;

    public:
        using SE::Behavior::Behavior;

        void start() override;
        void update() override;
    };
}