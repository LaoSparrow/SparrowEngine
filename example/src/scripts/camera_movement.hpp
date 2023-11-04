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
    float speed = 1.0f;
    float acc_speed = 5.0f;
    float mouse_sensitivity = 0.2f;

    using SparrowEngine::Behavior::Behavior;

    void update() override {
        GLFWwindow *w = SparrowEngine::GameWindow::GetCurrentActiveWindow()->glfw_window;
        auto parent = game_object.lock();
        glm::vec3 front;
        front.x =  sin(glm::radians(parent->transform.rotation.y)) * cos(glm::radians(parent->transform.rotation.x));
        front.y =  sin(glm::radians(parent->transform.rotation.x));
        front.z = -cos(glm::radians(parent->transform.rotation.y)) * cos(glm::radians(parent->transform.rotation.x));
        front = glm::normalize(front);
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
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
            float offset_x = cursor_x - last_cursor_x;
            float offset_y = last_cursor_y - cursor_y;
            last_cursor_x = cursor_x;
            last_cursor_y = cursor_y;

            offset_x *= mouse_sensitivity;
            offset_y *= mouse_sensitivity;

            parent->transform.rotation.y += offset_x;
            parent->transform.rotation.x += offset_y;
            parent->transform.rotation.x = std::max(std::min(parent->transform.rotation.x, 89.9f), -89.9f);
        }
    }
};