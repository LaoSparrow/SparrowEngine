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
        std::string texture_path;

        ~Texture();

        void load(const std::string &res_url);
        void free();
        void use();

        static std::unordered_map<std::string, std::weak_ptr<Texture>> texture_cache; // image_path <-> texture
        static std::shared_ptr<Texture> create_texture(std::string image_path);
    };

    Texture::~Texture() {
        free();
    }

    void Texture::load(const std::string &res_url) {
        if (res_url.starts_with("se://texture?color=")) {
            unsigned char data[sizeof(unsigned int)];
            sscanf_s(res_url.c_str(), "se://texture?color=%x", (unsigned int*)data);
            std::swap(data[0], data[2]);

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D,
                         0, // mipmap level
                         GL_RGB,
                         1, 1, // width height
                         0, // always zero
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
            texture_path = res_url;
            return;
        }

        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(res_url.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cout << "Failed to load texture, path: " << res_url << std::endl;
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
                         res_url.ends_with(".png") ? GL_RGBA : GL_RGB,
                         width, height,
                         0, // always zero
                         res_url.ends_with(".png") ? GL_RGBA : GL_RGB,
                         GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        stbi_image_free(data);
        texture_path = res_url;
    }

    void Texture::free() {
        if (id)
            glDeleteTextures(1, &id);
        id = 0;
        texture_path = "";
    }

    void Texture::use() {
        if (id)
            glBindTexture(GL_TEXTURE_2D, id);
    }

    std::unordered_map<std::string, std::weak_ptr<Texture>> Texture::texture_cache;

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