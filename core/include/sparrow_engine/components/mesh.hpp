#pragma once

#include "sparrow_engine/behavior.hpp"

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include <vector>
#include <memory>

namespace SparrowEngine::Components {

    struct MeshData {
        glm::vec3 position;
    };

    class Mesh : public SparrowEngine::Behavior {
    private:
        GLuint vao;
        GLuint vbo;

        GLuint shader_program;

        void allocate_gl_objects();

    public:
        std::vector<MeshData> mesh_data;

        explicit Mesh(std::weak_ptr<GameObject> obj);
        Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<MeshData> mesh_data);
        Mesh(std::weak_ptr<GameObject> obj, std::vector<MeshData>&& mesh_data);
        ~Mesh();

        void start() override;
        void render() override;
    };

}