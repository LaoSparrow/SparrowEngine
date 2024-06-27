#pragma once

#include "glad/glad.h"

#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace SE {

    class Texture {
    public:
        GLuint id{0};
        std::string texture_path;

        ~Texture();

        void load(const std::string &res_url);
        void free();
        void use();

        static std::unordered_map<std::string, std::weak_ptr<Texture>> texture_cache; // image_path <-> texture
        static std::shared_ptr<Texture> create_texture(std::string image_path);
    };

}