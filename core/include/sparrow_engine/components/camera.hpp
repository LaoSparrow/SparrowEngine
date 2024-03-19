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

        float fov = 45.0f;

        glm::mat4 mat_projection;
        glm::mat4 mat_view;

        void update() override;
    };

    void Camera::update() {
        GameWindow* w = SparrowEngine::GameWindow::GetCurrentActiveWindow();
        mat_projection = glm::perspective(
            glm::radians(fov),
            (float)w->width/(float)w->height,
            0.1f, 100.0f);
        auto obj = game_object.lock();
        Transform t = obj->transform;
        glm::mat4 model_mat = obj->get_model_matrix_in_global();
        glm::vec3 pos(model_mat[3][0], model_mat[3][1], model_mat[3][2]);
        model_mat[3][0] = 0.0f;
        model_mat[3][1] = 0.0f;
        model_mat[3][2] = 0.0f;
        glm::vec3 rot = t.rotation;

        glm::vec3 front;
        front.x = -sin(glm::radians(rot.y)) * cos(glm::radians(rot.x));
        front.y =  sin(glm::radians(rot.x));
        front.z = -cos(glm::radians(rot.y)) * cos(glm::radians(rot.x));
//        glm::vec3 front = glm::normalize(glm::vec3(model_mat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f))); //TODO: solve gimbal lock
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        mat_view = glm::lookAt(pos, pos + front, up);

        w->mat_projection = mat_projection;
        w->mat_view = mat_view;
        Behavior::update();
    }

}