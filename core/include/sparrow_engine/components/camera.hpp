#pragma once

#include "../game_object.hpp"
#include "../game_window.hpp"
#include "../transform.hpp"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace SE::Components {

    class Camera : public SE::Behavior {
    public:
        using SE::Behavior::Behavior;

        float fov = 45.0f;

        glm::mat4 mat_projection;
        glm::mat4 mat_view;

        void update() override;
    };

}