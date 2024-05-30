#pragma once

#include "sparrow_engine/components/mesh.hpp"

namespace SparrowEngine::Example {

    class Constants {
    public:
        Constants() = delete;

        static std::initializer_list<SparrowEngine::Components::Mesh::Vertex> plane;
        static std::initializer_list<unsigned int> plane_vertex_indices;

        static std::initializer_list<SparrowEngine::Components::Mesh::Vertex> cube;
    };

}