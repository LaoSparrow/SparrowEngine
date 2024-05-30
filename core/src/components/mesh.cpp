#include "sparrow_engine/components/mesh.hpp"

using namespace SparrowEngine::Components;

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