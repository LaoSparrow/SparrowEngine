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

}