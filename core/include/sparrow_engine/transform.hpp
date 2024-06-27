#pragma once

#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <utility>

namespace SE {
    class Transform {
    public:
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};

        void set_euler_angles(float x, float y, float z);
    };
}