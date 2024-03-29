#pragma once

#include "texture.hpp"
#include "transform.hpp"
#include "game_window.hpp"
#include "material.hpp"
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


namespace SparrowEngine {

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

        void load_material(const std::shared_ptr<Material>& material);

        static std::unordered_map<std::pair<std::string, std::string>, std::weak_ptr<Shader>, SparrowEngine::Utils::pair_hash> shader_cache; // {vertex_shader_path,fragment_shader_path} <-> shader
        static std::shared_ptr<Shader> create_shader(std::string vs_src, std::string fs_src);
    };

    void Shader::load(std::string vertex_path, std::string fragment_path) {
        if (id)
            return;

        vertex_shader_path = vertex_path;
        fragment_shader_path = fragment_path;

        // 1. 从文件路径中获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // 保证 ifstream 对象可以抛出异常：
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // 打开文件
            vShaderFile.open(vertex_path);
            fShaderFile.open(fragment_path);
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件的缓冲内容到数据流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到string
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. 编译着色器
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

// 顶点着色器
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
// 打印编译错误（如果有的话）
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

// 片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
// 打印编译错误（如果有的话）
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
// 打印连接错误（如果有的话）
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        int active_uniforms_size;
        glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &active_uniforms_size);
        for (int i = 0; i < active_uniforms_size; i++) {
            char name_buf[101];
            GLint uniform_size;
            GLenum uniform_type;
            glGetActiveUniform(id,
                               i,
                               100,
                               NULL,
                               &uniform_size,
                               &uniform_type,
                               name_buf);
            if (strcmp(name_buf, "projection") == 0) field_presented.projection = true;
            if (strcmp(name_buf, "view") == 0) field_presented.view = true;
            if (strcmp(name_buf, "model") == 0) field_presented.model = true;
            if (strcmp(name_buf, "normal_matrix") == 0) field_presented.normal_matrix = true;
            if (strcmp(name_buf, "view_position") == 0) field_presented.view_position = true;
        }
    }

    void Shader::free() {
        if (id)
            glDeleteProgram(id);
        id = 0;
        vertex_shader_path = "";
        fragment_shader_path = "";
    }

    Shader::~Shader() {
        free();
    }

    void Shader::use() {
        if (id)
            glUseProgram(id);
    }

    void Shader::set_int(const char *name, int value) {
        use();
        glUniform1i(glGetUniformLocation(id, name), value);
    }

    void Shader::set_float(const char *name, float value) {
        use();
        glUniform1f(glGetUniformLocation(id, name), value);
    }

    void Shader::set_vec3(const char *name, glm::vec3 value) {
        use();
        glUniform3fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
    }

    void Shader::set_vec4(const char *name, glm::vec4 value) {
        use();
        glUniform4fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
    }

    void Shader::set_mat3(const char *name, glm::mat3 &value) {
        use();
        glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::set_mat4(const char *name, glm::mat4 &value) {
        use();
        glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::push_mats(glm::mat4 model_matrix) {
        use();
        GameWindow *w = GameWindow::GetCurrentActiveWindow();
        if (field_presented.projection)
            set_mat4("projection", w->mat_projection);
        if (field_presented.view)
            set_mat4("view", w->mat_view);
        if (field_presented.model)
            set_mat4("model", model_matrix);
        if (field_presented.normal_matrix) {
            glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(model_matrix)));
            set_mat3("normal_matrix", normal_matrix);
        }
        if (field_presented.view_position)
            set_vec3("view_position", glm::inverse(w->mat_view)[3]);
    }

    void Shader::load_material(const std::shared_ptr<Material>& material) {
        use();
        int texture_index = 0;
        for (const auto &i : material->parameters) {
            auto field_name = i.first;
            auto value = i.second;

            if (value.type() == typeid(std::shared_ptr<Texture>)) {
                auto t = std::any_cast<std::shared_ptr<Texture>>(value);

                set_int(field_name.c_str(), texture_index);
                glActiveTexture(GL_TEXTURE0 + texture_index);
                t->use();

                texture_index++;
                continue;
            }
            if (value.type() == typeid(int)) {
                set_int(field_name.c_str(), std::any_cast<int>(value));
                continue;
            }
            if (value.type() == typeid(float)) {
                set_float(field_name.c_str(), std::any_cast<float>(value));
                continue;
            }
            if (value.type() == typeid(glm::vec3)) {
                set_vec3(field_name.c_str(), std::any_cast<glm::vec3>(value));
                continue;
            }
            if (value.type() == typeid(glm::vec4)) {
                set_vec4(field_name.c_str(), std::any_cast<glm::vec4>(value));
                continue;
            }
            if (value.type() == typeid(glm::mat3)) {
                set_mat3(field_name.c_str(), std::any_cast<glm::mat3&>(value));
                continue;
            }
            if (value.type() == typeid(glm::mat4)) {
                set_mat4(field_name.c_str(), std::any_cast<glm::mat4&>(value));
                continue;
            }
        }
    }

    std::unordered_map<std::pair<std::string, std::string>, std::weak_ptr<Shader>, SparrowEngine::Utils::pair_hash> Shader::shader_cache;

    std::shared_ptr<Shader> Shader::create_shader(std::string vs_src, std::string fs_src) {
        auto cache = shader_cache.find({ vs_src, fs_src });
        if (cache != shader_cache.end()) {
            auto sp = cache->second.lock();
            if (sp)
                return sp;
            else
                shader_cache.erase(cache->first);
        }

        auto sp = std::make_shared<Shader>();
        sp->load(vs_src, fs_src);
        shader_cache.insert({{ vs_src, fs_src }, std::weak_ptr(sp)});
        return sp;
    }
}