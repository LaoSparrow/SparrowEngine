#pragma once

#include "sparrow_engine/components/mesh.hpp"

namespace SE::Example {

    class Constants {
    public:
        Constants() = delete;

        static std::initializer_list<SE::Components::Mesh::Vertex> plane;
        static std::initializer_list<unsigned int> plane_vertex_indices;

        static std::initializer_list<SE::Components::Mesh::Vertex> cube;
    };

}