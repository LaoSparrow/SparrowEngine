#pragma once

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#include <string>
#include <iostream>
#include <memory>

namespace SparrowEngine {

    class Texture {
    public:
        GLuint id{0};

        static std::unordered_map<std::string, std::weak_ptr<Texture>> texture_cache; // image_path <-> texture

        ~Texture();

        void load(const std::string &image_path);
        void free();
        void use();

        static std::shared_ptr<Texture> create_texture(const std::string image_path);
    };

    Texture::~Texture() {
        free();
    }

    std::unordered_map<std::string, std::weak_ptr<Texture>> Texture::texture_cache;

    void Texture::load(const std::string &image_path) {
        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cout << "Failed to load texture, path: " << image_path << std::endl;
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D,
                         0, // mipmap level
                         image_path.ends_with(".png") ? GL_RGBA : GL_RGB,
                         width, height,
                         0, // always zero
                         image_path.ends_with(".png") ? GL_RGBA : GL_RGB,
                         GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        stbi_image_free(data);
    }

    void Texture::free() {
        if (id)
            glDeleteTextures(1, &id);
        id = 0;
    }

    void Texture::use() {
        if (id)
            glBindTexture(GL_TEXTURE_2D, id);
    }

    std::shared_ptr<Texture> Texture::create_texture(const std::string image_path) {
        auto cache = texture_cache.find(image_path);
        if (cache != texture_cache.end()) {
            auto sp = cache->second.lock();
            if (sp)
                return sp;
            else
                texture_cache.erase(cache->first);
        }

        auto sp = std::make_shared<Texture>();
        sp->load(image_path);
        texture_cache.insert({image_path, std::weak_ptr(sp)});
        return sp;
    }

}