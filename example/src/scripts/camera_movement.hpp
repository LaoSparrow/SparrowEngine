#pragma once

#include "sparrow_engine/behavior.hpp"


namespace SparrowEngine::Example::Scripts {
    class CameraMovement : public SparrowEngine::Behavior {
    private:
        double last_cursor_x = 0.0, last_cursor_y = 0.0;
        bool is_cursor_disabled = false;

    public:
        enum CameraMode {
            fps,
            free
        };

        float speed = 1.0f;
        float acc_speed = 5.0f;
        float mouse_sensitivity = 0.2f;
        CameraMode mode = CameraMode::fps;
        float yaw = 0.0f;
        float pitch = 0.0f;

        using SparrowEngine::Behavior::Behavior;

        void update() override;
    };
}