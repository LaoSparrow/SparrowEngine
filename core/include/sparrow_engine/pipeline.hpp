#pragma once

#include <glm/mat4x4.hpp>

namespace SE {
    class Pipeline {
    public:
        glm::mat4 mat_projection{1.0f};
        glm::mat4 mat_view{1.0f};

        static Pipeline current;
    };
}

