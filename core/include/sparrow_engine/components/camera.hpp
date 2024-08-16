#pragma once

#include "../game_object.hpp"
#include "../game_window.hpp"
#include "../transform.hpp"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace SE::Components {

    class Camera : public SE::Behavior {
    public:
        enum CameraType : int {
            Perspective = 0,
            Orthographic = 1,
        };

        using SE::Behavior::Behavior;

        CameraType type = CameraType::Perspective;
        float fov = 45.0f;
        float near = 0.1f;
        float far = 10000.0f;

        glm::mat4 mat_projection;
        glm::mat4 mat_view;

        void update() override;
    };

}