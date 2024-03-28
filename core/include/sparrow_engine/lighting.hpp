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
        enum LightType : int {
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

    void Lighting::reallocate_and_update_data() {
        size_t size_before_reallocation = light_data.size();
        light_data.emplace_back();
        light_data.resize(light_data.capacity(), LightData());
        for (size_t i = size_before_reallocation; i < light_data.size(); i++)
            available_slots.push(i);

        glDeleteBuffers(1, &ssbo_id);

        glGenBuffers(1, &ssbo_id);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightData) * light_data.size(), light_data.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_id);
    }

    Lighting::~Lighting() {
        if (ssbo_id != 0)
            glDeleteBuffers(1, &ssbo_id);
    }

    void Lighting::initialize() {
        for (size_t i = 0; i < light_data.size(); i++)
            available_slots.push(i);

        glGenBuffers(1, &ssbo_id);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightData) * light_data.size(), light_data.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_id);
    }

    void Lighting::use() {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_id);
    }

    int Lighting::next_available_slot() {
        if (available_slots.empty())
            reallocate_and_update_data();

        unsigned int slot = available_slots.front();
        available_slots.pop();
        return slot;
    }

    void Lighting::return_slot(int index) {
        light_data[index].type = LightType::None;
        push_data(index);
        available_slots.push(index);
    }

    void Lighting::push_data(int index) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(LightData) * index, sizeof(LightData), &light_data[index]);
    }

    Lighting::LightData& Lighting::get_data(int index) {
        return light_data[index];
    }

}

