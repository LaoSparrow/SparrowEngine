#include "sparrow_engine/components/camera.hpp"

using namespace SE::Components;

void Camera::update() {
    GameWindow* w = SE::GameWindow::GetCurrent();
    if (type == Perspective) {
        mat_projection = glm::perspective(
            glm::radians(fov),
            (float)w->width/(float)w->height,
            near, far);
    }
    else {
        float width = fov * (float)w->width/(float)w->height;
        mat_projection = glm::ortho(
            -width/2, width/2,
            -fov/2, fov/2,
            near, far);
    }
    auto obj = game_object.lock();
    glm::mat4 model_mat = obj->get_model_matrix_in_global();

    glm::vec3 pos(model_mat[3]);
    model_mat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 front = glm::normalize(glm::vec3(model_mat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(model_mat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))));
    glm::vec3 up = glm::normalize(glm::cross(right, front));
    mat_view = glm::lookAt(pos, pos + front, up);

    w->mat_projection = mat_projection;
    w->mat_view = mat_view;
    Behavior::update();
}