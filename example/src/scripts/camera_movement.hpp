#pragma once

#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/game_window.hpp"

#include "glm/glm.hpp"
#include "sparrow_engine/time.hpp"

class CameraMovement : public SparrowEngine::Behavior {
private:
    double last_cursor_x = 0.0, last_cursor_y = 0.0;
    bool is_backslash_pressed = false;
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

    void update() override {
        GLFWwindow *w = SparrowEngine::GameWindow::GetCurrentActiveWindow()->glfw_window;
        auto parent = game_object.lock();
        glm::mat4 model_mat = parent->get_model_matrix_in_global();

        glm::vec3 pos(model_mat[3]);
        model_mat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        glm::vec3 front = glm::normalize(glm::vec3(model_mat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(model_mat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))));
        glm::vec3 up = glm::normalize(glm::cross(right, front));

        auto delta_time = (float)SparrowEngine::Time::GetDeltaTime();
        float current_speed = speed;
        if (glfwGetKey(w, GLFW_KEY_LEFT_SHIFT))
            current_speed = acc_speed;
        if (glfwGetKey(w, GLFW_KEY_W))
            parent->transform.position += front * current_speed * delta_time;
        if (glfwGetKey(w, GLFW_KEY_S))
            parent->transform.position -= front * current_speed * delta_time;
        if (glfwGetKey(w, GLFW_KEY_A))
            parent->transform.position -= right * current_speed * delta_time;
        if (glfwGetKey(w, GLFW_KEY_D))
            parent->transform.position += right * current_speed * delta_time;
        if (glfwGetKey(w, GLFW_KEY_E))
            parent->transform.position += up * current_speed * delta_time;
        if (glfwGetKey(w, GLFW_KEY_Q))
            parent->transform.position -= up * current_speed * delta_time;

        if (glfwGetKey(w, GLFW_KEY_BACKSLASH)) {
            if (!is_backslash_pressed) {
                is_cursor_disabled = !is_cursor_disabled;
                glfwSetInputMode(w, GLFW_CURSOR, is_cursor_disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
                glfwGetCursorPos(w, &last_cursor_x, &last_cursor_y);
            }
            is_backslash_pressed = true;
        }
        else
            is_backslash_pressed = false;

        if (is_cursor_disabled) {
            double cursor_x, cursor_y;
            glfwGetCursorPos(w, &cursor_x, &cursor_y);
            float offset_x = last_cursor_x - cursor_x;
            float offset_y = last_cursor_y - cursor_y;
            last_cursor_x = cursor_x;
            last_cursor_y = cursor_y;

            offset_x *= mouse_sensitivity;
            offset_y *= mouse_sensitivity;

            switch (mode) {
                case CameraMode::fps:
                    yaw += offset_x;
                    pitch += offset_y;
                    pitch = std::min(std::max(pitch, -90.0f), 90.0f);

                    parent->transform.rotation = glm::quat(glm::vec3(
                        glm::radians(pitch),
                        glm::radians(yaw),
                        0.0f));
                    break;

                case CameraMode::free:
                    parent->transform.rotation *=
                        glm::angleAxis(glm::radians(offset_y), glm::vec3(1.0f, 0.0f, 0.0f)) *
                        glm::angleAxis(glm::radians(offset_x), glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
            }
        }
    }
};