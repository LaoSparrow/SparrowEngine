#pragma once

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include <vector>
#include <queue>

#define LIGHTING_DATA_DEFAULT_CAPACITY 32

namespace SparrowEngine {

    class Lighting {
    private:
        void reallocate_and_update_data();

    public:
        enum class LightType : int {
            None = 0,
            DirectionalLight = 1,
            PointLight = 2,
            SpotLight = 3,
            AreaLight = 4 // TODO: AreaLight
        };
        struct LightData {
            LightType type = LightType::None;

            alignas(16) glm::vec3 position{};
            alignas(16) glm::vec3 direction{};

            alignas(16) glm::vec3 ambient{};
            alignas(16) glm::vec3 diffuse{};
            alignas(16) glm::vec3 specular{};

            float constant{};
            float linear{};
            float quadratic{};

            float cut_off{};
            float outer_cut_off{};
        };

        GLuint ssbo_id{0};
        std::vector<LightData> light_data{LIGHTING_DATA_DEFAULT_CAPACITY, LightData()};
        std::queue<int> available_slots;


        Lighting() = default;
        ~Lighting();

        void initialize();
        void use();
        int next_available_slot();
        void return_slot(int index);
        void push_data(int index);
        LightData& get_data(int index);
    };

}

