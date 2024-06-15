#pragma once

#include "sparrow_engine/behavior.hpp"

#include "glm/gtc/quaternion.hpp"
#include "glm/vec3.hpp"

namespace SparrowEngine::Example::Scripts {
    class TransformModification : public SparrowEngine::Behavior {
    private:
        glm::vec3 euler_angle;
        glm::quat orig_quat;

    public:
        using SparrowEngine::Behavior::Behavior;

        void start() override;
        void update() override;
    };
}