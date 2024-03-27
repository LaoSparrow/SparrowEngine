#pragma once

#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/lighting.hpp"
#include "sparrow_engine/game_window.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"


namespace SparrowEngine::Components {

    class Light : public SparrowEngine::Behavior {
    private:
        int data_slot{-1};

    public:
        void start() override;
        void update() override;

        Lighting::LightData& get_light();

        ~Light();
    };

    void Light::start() {
        data_slot = Scene::GetCurrentScene()->lighting.next_available_slot();
        Behavior::start();
    }

    void Light::update() {
        auto obj = game_object.lock();
        glm::mat4 model_mat = obj->get_model_matrix_in_global();
        get_light().position = glm::vec3(model_mat[3]);
        glm::mat3 rotational_mat(model_mat);
        for (int i = 0; i < 3; i++) {
            rotational_mat[i] /= glm::length(glm::vec3(rotational_mat[i]));
        }
        get_light().direction = rotational_mat * glm::vec3(0.0f, 0.0f, 1.0f);


        Scene::GetCurrentScene()->lighting.push_data(data_slot);
        Behavior::update();
    }

    Light::~Light() {
        // Will cause problem when it isn't deconstructed inside its parent scene
        // TODO: fix returning slot
        Scene::GetCurrentScene()->lighting.return_slot(data_slot);
        data_slot = -1;
    }

    Lighting::LightData& Light::get_light() {
        return Scene::GetCurrentScene()->lighting.get_data(data_slot);
    }
}