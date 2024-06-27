#pragma once

#include "transform.hpp"
#include "game_window.hpp"
#include "utils.hpp"

#include "glad/glad.h"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <any>


namespace SE {

    class Shader {
    private:

        struct {
            bool projection{false};
            bool view{false};
            bool model{false};
            bool normal_matrix{false};
            bool view_position{false};
        } field_presented;

    public:
        GLuint id{0};

        std::string vertex_shader_path;
        std::string fragment_shader_path;

        Shader() = default;
        ~Shader();

        void load(std::string vertex_path, std::string fragment_path);
        void free();
        void push_mats(glm::mat4 model_matrix = glm::mat4(1.0f));
        void use();

        void set_int(const char *name, int value);
        void set_float(const char *name, float value);
        void set_vec3(const char *name, glm::vec3 value);
        void set_vec4(const char *name, glm::vec4 value);
        void set_mat3(const char *name, glm::mat3 &value);
        void set_mat4(const char *name, glm::mat4 &value);

        static std::unordered_map<std::pair<std::string, std::string>, std::weak_ptr<Shader>, SE::Utils::pair_hash> shader_cache; // {vertex_shader_path,fragment_shader_path} <-> shader
        static std::shared_ptr<Shader> create_shader(std::string vs_src, std::string fs_src);
    };

}