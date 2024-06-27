#pragma once

#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/lighting.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"


namespace SE::Components {

    class Light : public SE::Behavior {
    private:
        int data_slot{-1};
        std::weak_ptr<Lighting> lighting_parent;

    public:
        Lighting::LightType type = Lighting::LightType::None;

        glm::vec3 ambient{};
        glm::vec3 diffuse{};
        glm::vec3 specular{};

        float constant{};
        float linear{};
        float quadratic{};

        float cut_off{};
        float outer_cut_off{};

        void start() override;
        void update() override;

        Lighting::LightData& get_light();

        using SE::Behavior::Behavior;
        ~Light();
    };
}