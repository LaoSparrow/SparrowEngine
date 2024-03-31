#pragma once

#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/material.hpp"

#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>
#include <memory>
#include <string>

namespace SparrowEngine::Components {

    class Mesh : public SparrowEngine::Behavior {
    private:
        GLuint vao{0};
        GLuint vbo{0};
        GLuint ebo{0};

        void allocate_gl_objects();
        void make_current_context();

    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec2 texture_coordinate;
            glm::vec3 normal;
        };
        
        std::vector<Vertex> vertices;
        std::vector<unsigned int> vertex_indices;

        std::shared_ptr<Material> material;

        using SparrowEngine::Behavior::Behavior;

        Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<Vertex> mesh_data);

        Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<Vertex> mesh_data,
             std::initializer_list<unsigned int> vertex_indices);

        Mesh(std::weak_ptr<GameObject> obj, std::vector<Vertex> &&mesh_data);

        ~Mesh();

        void start() override;

        void render() override;
    };

    void Mesh::allocate_gl_objects() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        if (!vertex_indices.empty())
            glGenBuffers(1, &ebo);
    }

    void Mesh::make_current_context() {
        glBindVertexArray(vao);
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        if (ebo)
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//        material->shader->use();
    }

    Mesh::Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<Vertex> mesh_data)
        : Behavior(std::move(obj)), vertices(mesh_data) {}

    Mesh::Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<Vertex> mesh_data,
               std::initializer_list<unsigned int> vertex_indices)
               : Behavior(std::move(obj)), vertices(mesh_data), vertex_indices(vertex_indices) {}

    Mesh::Mesh(std::weak_ptr<GameObject> obj, std::vector<Vertex> &&mesh_data)
        : Behavior(std::move(obj)), vertices(mesh_data) {}

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
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        if (ebo) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertex_indices.size(), vertex_indices.data(), GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, texture_coordinate));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        Behavior::start();
    }

    void Mesh::render() {
        make_current_context();
        material->use();
        material->shader->push_mats(game_object.lock()->get_model_matrix_in_global());
        if (ebo)
            glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);
        Behavior::render();
    }

}