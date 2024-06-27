#include "camera_movement.hpp"

#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/input.hpp"

#include "glm/glm.hpp"
#include "sparrow_engine/time.hpp"

using namespace SE::Example::Scripts;
using Input = SE::Input;

void CameraMovement::update() {
    GLFWwindow *w = SE::GameWindow::GetCurrent()->glfw_window;
    auto parent = game_object.lock();
    glm::mat4 model_mat = parent->get_model_matrix_in_global();

    glm::vec3 pos(model_mat[3]);
    model_mat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 front = glm::normalize(glm::vec3(model_mat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(model_mat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    auto delta_time = (float)SE::Time::GetDeltaTime();
    float current_speed = speed;
    if (Input::KeyPressed(GLFW_KEY_LEFT_SHIFT))
        current_speed = acc_speed;
    if (Input::KeyPressed(GLFW_KEY_W))
        parent->transform.position += front * current_speed * delta_time;
    if (Input::KeyPressed(GLFW_KEY_S))
        parent->transform.position -= front * current_speed * delta_time;
    if (Input::KeyPressed(GLFW_KEY_A))
        parent->transform.position -= right * current_speed * delta_time;
    if (Input::KeyPressed(GLFW_KEY_D))
        parent->transform.position += right * current_speed * delta_time;
    if (Input::KeyPressed(GLFW_KEY_E))
        parent->transform.position += up * current_speed * delta_time;
    if (Input::KeyPressed(GLFW_KEY_Q))
        parent->transform.position -= up * current_speed * delta_time;

    if (Input::KeyTriggered(GLFW_KEY_BACKSLASH)) {
        Input::SetInputMode(GLFW_CURSOR,
                            Input::GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_NORMAL ?
                            GLFW_CURSOR_DISABLED :
                            GLFW_CURSOR_NORMAL);
    }

    if (Input::GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        glm::dvec2 delta = Input::GetDeltaCursorPosition();
        delta *= mouse_sensitivity;

        switch (mode) {
            case CameraMode::fps:
                yaw -= delta.x;
                pitch += delta.y;
                pitch = std::min(std::max(pitch, -90.0f), 90.0f);

                parent->transform.rotation = glm::quat(glm::vec3(
                    glm::radians(pitch),
                    glm::radians(yaw),
                    0.0f));
                break;

            case CameraMode::free:
                parent->transform.rotation *=
                    glm::angleAxis(glm::radians((float)delta.y), glm::vec3(1.0f, 0.0f, 0.0f)) *
                    glm::angleAxis(glm::radians((float)-delta.x), glm::vec3(0.0f, 1.0f, 0.0f));
                break;
        }
    }
}
