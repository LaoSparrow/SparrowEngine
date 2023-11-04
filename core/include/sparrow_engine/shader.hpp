#pragma once

#include "texture.hpp"

#include "glad/glad.h"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifndef MAX_TEXTURES
#define MAX_TEXTURES 16
#endif

namespace SparrowEngine {

    class Shader {
    private:
        bool is_loaded{false};
        Texture textures[MAX_TEXTURES];

        bool is_projection_present{false};
        bool is_view_present{false};
        bool is_model_present{false};

    public:
        GLuint id{0};

        static GLuint current_active_id;

        Shader() = default;
        ~Shader();

        void initialize(const char *vertex_path, const char* fragment_path);
        void load_texture(unsigned int slot, std::string &image_path);
        void push_mats(Transform model_transform = Transform());
        void use();

        void set_int(const char *name, int value);
        void set_mat4(const char *name, glm::mat4 &value);
    };

    GLuint Shader::current_active_id = 0;

    void Shader::initialize(const char *vertex_path, const char *fragment_path) {
        if (is_loaded)
            return;
        is_loaded = true;

        // 1. 从文件路径中获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // 保证ifstream对象可以抛出异常：
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
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
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
            if (strcmp(name_buf, "projection") == 0) is_projection_present = true;
            if (strcmp(name_buf, "view") == 0) is_view_present = true;
            if (strcmp(name_buf, "model") == 0) is_model_present = true;

            if (strncmp(name_buf, "texture_", 8) != 0)
                continue;
            unsigned int texture_slot;
            sscanf_s(name_buf, "texture_%u", &texture_slot);
            set_int(name_buf, texture_slot);
        }
    }

    Shader::~Shader() {
        if (is_loaded)
            glDeleteProgram(id);
    }

    void Shader::load_texture(unsigned int slot, std::string &image_path) {
        textures[slot].load(image_path);
    }

    void Shader::use() {
        if (is_loaded) {
//            if (current_active_id == id)
//                return;

            glUseProgram(id);
            for (int i = 0; i < MAX_TEXTURES; i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                textures[i].use();
            }
            current_active_id = id;
        }
    }

    void Shader::set_int(const char *name, int value) {
        use();
        glUniform1i(glGetUniformLocation(id, name), value);
    }

    void Shader::set_mat4(const char *name, glm::mat4 &value) {
        use();
        glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::push_mats(Transform model_transform) {
        use();
        GameWindow *w = GameWindow::GetCurrentActiveWindow();
        if (is_projection_present)
            set_mat4("projection", w->mat_projection);
        if (is_view_present)
            set_mat4("view", w->mat_view);
        if (is_model_present) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), model_transform.position);
            model = glm::rotate(model, glm::radians(model_transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(model_transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(model_transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, model_transform.scale);
            set_mat4("model", model);
        }
    }

}