#pragma once

#include "../game_object.hpp"
#include "../game_window.hpp"
#include "../transform.hpp"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace SparrowEngine::Components {

    class Camera : public SparrowEngine::Behavior {
    public:
        using SparrowEngine::Behavior::Behavior;

        glm::mat4 mat_projection;
        glm::mat4 mat_view;

        void update() override;
    };

    void Camera::update() {
        GameWindow* w = SparrowEngine::GameWindow::GetCurrentActiveWindow();
        mat_projection = glm::perspective(
            glm::radians(45.0f),
            (float)w->width/(float)w->height,
            0.1f, 100.0f);
        Transform t = game_object.lock()->transform;
        glm::vec3 pos = t.position;
        glm::vec3 rot = t.rotation;

//        mat_view = glm::translate(glm::mat4(1.0f), glm::vec3(-pos.x, -pos.y, -pos.z));
        glm::vec3 front;
        front.x =  sin(glm::radians(rot.y)) * cos(glm::radians(rot.x));
        front.y =  sin(glm::radians(rot.x));
        front.z = -cos(glm::radians(rot.y)) * cos(glm::radians(rot.x));
        front = glm::normalize(front);
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        mat_view = glm::lookAt(pos, pos + front, up);

        w->mat_projection = mat_projection;
        w->mat_view = mat_view;
        Behavior::update();
    }

}