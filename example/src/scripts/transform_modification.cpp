#include "transform_modification.hpp"

#include "sparrow_engine/game_object.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

using namespace SparrowEngine::Example::Scripts;

void TransformModification::update() {
    auto parent_obj = game_object.lock();
    if (ImGui::Begin((parent_obj->name + " transform").c_str())) {
        ImGui::SliderFloat3("position", (float *)&parent_obj->transform.position, -5.0f, 5.0f);
        ImGui::SliderFloat3("scale", (float *)&parent_obj->transform.scale, 0.0f, 5.0f);
        ImGui::SliderFloat4("rotation", (float *)&parent_obj->transform.rotation, 0.0f, 1.0f);

        glm::vec3 euler_angle = glm::degrees(glm::eulerAngles(parent_obj->transform.rotation));
        if (ImGui::SliderFloat3("euler angle", glm::value_ptr(euler_angle), -360.0f, 360.0f))
            parent_obj->transform.rotation = glm::quat(glm::radians(euler_angle));

        ImGui::End();
    }
}
