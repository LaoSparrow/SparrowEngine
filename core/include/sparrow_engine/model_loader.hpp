#pragma once

#include "components/mesh.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

namespace SparrowEngine {

    using Vertex = Components::Mesh::Vertex;

    class ModelLoader {
    public:
        struct MeshData {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            struct {
                std::vector<std::string> diffuse;
                std::vector<std::string> specular;
                std::vector<std::string> emission;
            } texture_paths;
        };
        std::string directory;

    private:
        void fill_vertices(const aiScene *scene, const aiMesh *mesh, std::vector<Vertex> &vertices);
        void fill_indices(const aiScene *scene, const aiMesh *mesh, std::vector<unsigned int> &indices);
        void fill_texture_paths(const aiScene *scene, const aiMesh *mesh, MeshData &data);

        void load_material_textures(aiMaterial *material, aiTextureType type, std::vector<std::string> &texture_paths);

    public:
        std::vector<MeshData> meshes;

        void load_model(std::string path, aiPostProcessSteps post_processes = (aiPostProcessSteps)0);

    };

}