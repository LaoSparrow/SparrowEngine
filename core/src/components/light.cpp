#include "sparrow_engine/components/light.hpp"

using namespace SparrowEngine::Components;

void Light::start() {
    lighting_parent = Scene::GetCurrent()->lighting;
    data_slot = Scene::GetCurrent()->lighting->next_available_slot();
}

void Light::update() {
    auto lighting = lighting_parent.lock();
    if (!lighting)
        return;

    auto data = &lighting->get_data(data_slot);

    data->type = type;

    auto obj = game_object.lock();
    glm::mat4 model_mat = obj->get_model_matrix_in_global();
    glm::mat3 rotational_mat(model_mat);
    for (int i = 0; i < 3; i++) {
        rotational_mat[i] /= glm::length(glm::vec3(rotational_mat[i]));
    }
    data->position = glm::vec3(model_mat[3]);
    data->direction = rotational_mat * glm::vec3(0.0f, 0.0f, 1.0f);

    data->ambient = ambient;
    data->diffuse = diffuse;
    data->specular = specular;

    data->constant = constant;
    data->linear = linear;
    data->quadratic = quadratic;

    data->cut_off = cut_off;
    data->outer_cut_off = outer_cut_off;

    lighting->push_data(data_slot);
    Behavior::update();
}

Light::~Light() {
    auto lighting = lighting_parent.lock();
    if (!lighting)
        return;

    lighting->return_slot(data_slot);
    data_slot = -1;
}

SparrowEngine::Lighting::LightData& Light::get_light() {
    auto lighting = lighting_parent.lock();
    // TODO: return sth when lighting doesn't present

    return lighting->get_data(data_slot);
}