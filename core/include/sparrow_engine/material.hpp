#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <any>

namespace SparrowEngine {

    class Material {
    public:
        std::unordered_map<std::string, std::any> parameters;

        Material() = default;
        Material(std::initializer_list<decltype(parameters)::value_type> params_init);

        static std::shared_ptr<Material> create_material(std::initializer_list<decltype(parameters)::value_type> init_list);
    };

    Material::Material(std::initializer_list<decltype(parameters)::value_type> params_init)
        : parameters(params_init) {}

    std::shared_ptr<Material> Material::create_material(std::initializer_list<decltype(parameters)::value_type> init_list) {
        return std::move(std::make_shared<Material>(init_list));
    }
}