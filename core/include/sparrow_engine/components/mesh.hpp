#pragma once

#include "sparrow_engine/behavior.hpp"
#include "sparrow_engine/material.hpp"

#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>
#include <memory>
#include <string>

namespace SE::Components {

    class Mesh : public SE::Behavior {
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

        using SE::Behavior::Behavior;

        Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<Vertex> mesh_data);

        Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<Vertex> mesh_data,
             std::initializer_list<unsigned int> vertex_indices);

        Mesh(std::weak_ptr<GameObject> obj, std::vector<Vertex> &&mesh_data);

        ~Mesh();

        void start() override;

        void render() override;
    };

}