#include "sparrow_engine/shader.hpp"

using namespace SparrowEngine;

void Shader::load(std::string vertex_path, std::string fragment_path) {
    if (id)
        return;

    vertex_shader_path = vertex_path;
    fragment_shader_path = fragment_path;

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertex_path);
        fShaderFile.open(fragment_path);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

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

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

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