#pragma once

#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/shader.hpp"

#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>
#include <memory>
#include <string>

namespace SparrowEngine::Components {

    struct MeshData {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_coordinate;
    };

    class Mesh : public SparrowEngine::Behavior {
    private:
        GLuint vao{0};
        GLuint vbo{0};
        GLuint ebo{0};

        void allocate_gl_objects();
        void make_current_context();

    public:
        std::vector<MeshData> mesh_data;
        std::vector<unsigned int> vertex_indices;

        Shader shader;

        using SparrowEngine::Behavior::Behavior;

        Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<MeshData> mesh_data);

        Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<MeshData> mesh_data,
             std::initializer_list<unsigned int> vertex_indices);

        Mesh(std::weak_ptr<GameObject> obj, std::vector<MeshData> &&mesh_data);

        ~Mesh();

        void start() override;

        void render() override;

        std::shared_ptr<Mesh> use_shader(const char *vertex_path, const char *fragment_path);
        std::shared_ptr<Mesh> set_texture(std::string field_name, std::string image_path);
    };

    void Mesh::allocate_gl_objects() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        if (!vertex_indices.empty())
            glGenBuffers(1, &ebo);
    }

    void Mesh::make_current_context() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        if (ebo)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        shader.use();
    }

    Mesh::Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<MeshData> mesh_data)
        : Behavior(std::move(obj)), mesh_data(mesh_data) {}

    Mesh::Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<MeshData> mesh_data,
               std::initializer_list<unsigned int> vertex_indices)
               : Behavior(std::move(obj)), mesh_data(mesh_data), vertex_indices(vertex_indices) {}

    Mesh::Mesh(std::weak_ptr<GameObject> obj, std::vector<MeshData> &&mesh_data)
        : Behavior(std::move(obj)), mesh_data(mesh_data) {}

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        if (ebo)
            glDeleteBuffers(1, &ebo);
        glBindVertexArray(0);
    }

    void Mesh::start() {
        allocate_gl_objects();
        make_current_context();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(MeshData) * mesh_data.size(), mesh_data.data(), GL_STATIC_DRAW);

        if (ebo) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertex_indices.size(), vertex_indices.data(), GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData),
                              (void *) offsetof(MeshData, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData),
                              (void *) offsetof(MeshData, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshData),
                              (void *) offsetof(MeshData, texture_coordinate));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        Behavior::start();
    }

    void Mesh::render() {
        make_current_context();
        shader.push_mats(game_object.lock()->get_model_matrix_in_global());
        if (ebo)
            glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, mesh_data.size());

        glBindVertexArray(0);
        Behavior::render();
    }

    std::shared_ptr<Mesh> Mesh::use_shader(const char *vertex_path, const char *fragment_path) {
        shader.initialize(vertex_path, fragment_path);
        return std::static_pointer_cast<Mesh>(Behavior::shared_from_this());
    }

    std::shared_ptr<Mesh> Mesh::set_texture(std::string field_name, std::string image_path) {
        shader.load_texture(field_name, image_path);
        return std::static_pointer_cast<Mesh>(Behavior::shared_from_this());
    }

}