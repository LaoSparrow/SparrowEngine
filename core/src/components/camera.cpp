#include "sparrow_engine/components/camera.hpp"

using namespace SE::Components;

void Camera::post_update() {
    int width;
    int height;
    if (output) {
        width = output->width;
        height = output->height;
    }
    else {
        GameWindow* window = SE::GameWindow::GetCurrent();
        width = window->width;
        height = window->height;
    }

    if (type == Perspective) {
        mat_projection = glm::perspective(
            glm::radians(fov),
            (float)width / (float)height,
            near, far);
    }
    else {
        float horizontal_fov = fov * (float)width / (float)height;
        mat_projection = glm::ortho(
            -horizontal_fov / 2, horizontal_fov / 2,
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

    if (output) {
        output->framebuffer->pipeline.mat_projection = mat_projection;
        output->framebuffer->pipeline.mat_view = mat_view;
        Scene::GetCurrent()->rendering_texture_queue.push(output);
    }
    else {
        GameWindow* window = SE::GameWindow::GetCurrent();
        window->default_framebuffer->pipeline.mat_projection = mat_projection;
        window->default_framebuffer->pipeline.mat_view = mat_view;
    }

    Behavior::post_update();
}