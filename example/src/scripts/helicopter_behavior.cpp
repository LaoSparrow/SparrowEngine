#include "helicopter_behavior.hpp"

#include "sparrow_engine/game_object.hpp"
#include "sparrow_engine/time.hpp"
#include "sparrow_engine/input.hpp"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

using namespace SE::Example::Scripts;

//glm::quat rotate_to(glm::vec3 a, glm::vec3 b) {
//    a = glm::normalize(a);
//    b = glm::normalize(b);
//    float dot_result = glm::dot(a, b);
//    if (dot_result == 1.0f)
//        return {1.0f, 0.0f, 0.0f, 0.0f};
//    if (dot_result == -1.0f)
//        return glm::normalize(glm::angleAxis(glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));
//
//    glm::vec3 side_vector = glm::normalize(glm::cross(a, b));
//    return glm::angleAxis(std::acos(dot_result), side_vector);
//}

void HelicopterBehavior::update() {
    auto parent = game_object.lock();
    float propeller_force = 1.5f * (float)SE::Time::GetDeltaTime();
    external_force = glm::vec3(0.0f);

    glm::vec3 helicopter_up = parent->transform.rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 helicopter_front = parent->transform.rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 helicopter_right = parent->transform.rotation * glm::vec3(1.0f, 0.0f, 0.0f);

    if (SE::Input::KeyPressed(GLFW_KEY_W))
        parent->transform.rotation = glm::angleAxis(-propeller_force, helicopter_right) * parent->transform.rotation;
    if (SE::Input::KeyPressed(GLFW_KEY_A))
        parent->transform.rotation = glm::angleAxis(-propeller_force, helicopter_front) * parent->transform.rotation;
    if (SE::Input::KeyPressed(GLFW_KEY_S))
        parent->transform.rotation = glm::angleAxis(propeller_force, helicopter_right) * parent->transform.rotation;
    if (SE::Input::KeyPressed(GLFW_KEY_D))
        parent->transform.rotation = glm::angleAxis(propeller_force, helicopter_front) * parent->transform.rotation;
    if (SE::Input::KeyPressed(GLFW_KEY_Q))
        parent->transform.rotation = glm::angleAxis(propeller_force, helicopter_up) * parent->transform.rotation;
    if (SE::Input::KeyPressed(GLFW_KEY_E))
        parent->transform.rotation = glm::angleAxis(-propeller_force, helicopter_up) * parent->transform.rotation;


    float angle_between = std::acos(glm::dot(helicopter_up, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 side_vec = glm::cross(helicopter_up, glm::vec3(0.0f, 1.0f, 0.0f));
    if (side_vec != glm::vec3(0.0f))
        parent->transform.rotation = glm::angleAxis(angle_between * auto_correction * (float)SE::Time::GetDeltaTime(), glm::normalize(side_vec)) * parent->transform.rotation;

    throttle += (float)SE::Input::GetDeltaMouseScroll().y * 1000.0f;
    throttle = std::max(throttle, 0.0f);
    external_force.y = throttle;

    force = glm::vec3(0.0f, mass * gravitational_constant, 0.0f);
    external_force = parent->transform.rotation * external_force;
    force += external_force;

    if (parent->transform.position.y < ground_y + 0.01f) {
        if (velocity.x > 0.0f)
            force.x -= std::min(ground_friction, std::abs(velocity.x / (float)SE::Time::GetDeltaTime() * mass));
        else
            force.x += std::min(ground_friction, std::abs(velocity.x / (float)SE::Time::GetDeltaTime() * mass));
        if (velocity.z > 0.0f)
            force.z -= std::min(ground_friction, std::abs(velocity.z / (float)SE::Time::GetDeltaTime() * mass));
        else
            force.z += std::min(ground_friction, std::abs(velocity.z / (float)SE::Time::GetDeltaTime() * mass));
    }

    velocity += force / mass * (float)SE::Time::GetDeltaTime();
    velocity *= (1.0f - (air_resistance * (float)SE::Time::GetDeltaTime()));
    parent->transform.position += velocity * (float)SE::Time::GetDeltaTime();

    if (parent->transform.position.y < ground_y) {
        velocity.y = 0.0f;
        force.y = 0.0f;
        parent->transform.position.y = ground_y;
    }

    if (ImGui::Begin("Control Panel")) {
        ImGui::SliderFloat("auto correction", &auto_correction, 0.0f, 20.0f);
        ImGui::SliderFloat("air resistance", &air_resistance, 0.0f, 10.0f);
        ImGui::SliderFloat("gravitational constant", &gravitational_constant, -1000.0f, 0.0f);
        ImGui::Separator();
        ImGui::SliderFloat3("velocity", glm::value_ptr(velocity), -100.0f, 100.0f);
        ImGui::SliderFloat("height", &parent->transform.position.y, 0.0f, 1000.0f);
//        ImGui::SliderFloat3("force", glm::value_ptr(force), -1000.0f, 1000.0f);
//        ImGui::SliderFloat3("external force", glm::value_ptr(external_force), -1000.0f, 1000.0f);
        ImGui::SliderFloat("throttle", &throttle, 0.0f, 30000.0f);
    }
    ImGui::End();

    Behavior::update();
}
