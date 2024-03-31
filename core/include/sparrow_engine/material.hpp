#pragma once

#include "shader.hpp"
#include "texture.hpp"

#include <memory>
#include <unordered_map>
#include <string>
#include <any>
#include <utility>

namespace SparrowEngine {

    class Material {
    public:
        std::shared_ptr<Shader> shader;
        std::unordered_map<std::string, std::any> parameters;

        Material() = default;
        Material(std::shared_ptr<Shader> shader, std::initializer_list<decltype(parameters)::value_type> params_init);

        void use();

        static std::shared_ptr<Material> create_material(std::shared_ptr<Shader> shader, std::initializer_list<decltype(parameters)::value_type> init_list);
    };

    Material::Material(std::shared_ptr<Shader> shader, std::initializer_list<decltype(parameters)::value_type> params_init)
        : shader(std::move(shader)), parameters(params_init) {}

    void Material::use() {
        shader->use();
        int texture_index = 0;
        for (const auto &i : parameters) {
            auto field_name = i.first;
            auto value = i.second;

            if (value.type() == typeid(std::shared_ptr<Texture>)) {
                auto t = std::any_cast<std::shared_ptr<Texture>>(value);

                shader->set_int(field_name.c_str(), texture_index);
                glActiveTexture(GL_TEXTURE0 + texture_index);
                t->use();

                texture_index++;
                continue;
            }
            if (value.type() == typeid(int)) {
                shader->set_int(field_name.c_str(), std::any_cast<int>(value));
                continue;
            }
            if (value.type() == typeid(float)) {
                shader->set_float(field_name.c_str(), std::any_cast<float>(value));
                continue;
            }
            if (value.type() == typeid(glm::vec3)) {
                shader->set_vec3(field_name.c_str(), std::any_cast<glm::vec3>(value));
                continue;
            }
            if (value.type() == typeid(glm::vec4)) {
                shader->set_vec4(field_name.c_str(), std::any_cast<glm::vec4>(value));
                continue;
            }
            if (value.type() == typeid(glm::mat3)) {
                shader->set_mat3(field_name.c_str(), std::any_cast<glm::mat3&>(value));
                continue;
            }
            if (value.type() == typeid(glm::mat4)) {
                shader->set_mat4(field_name.c_str(), std::any_cast<glm::mat4&>(value));
                continue;
            }
        }
    }

    std::shared_ptr<Material> Material::create_material(std::shared_ptr<Shader> shader, std::initializer_list<decltype(parameters)::value_type> init_list) {
        return std::move(std::make_shared<Material>(std::move(shader), init_list));
    }
}