#include "sparrow_engine/model_loader.hpp"

using namespace SE;

void ModelLoader::load_model(std::string path, aiPostProcessSteps post_processes) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | post_processes);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = std::filesystem::path(path).parent_path().string();

    meshes.resize(scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh *mesh = scene->mMeshes[i];

        fill_vertices(scene, mesh, meshes[i].vertices);
        fill_indices(scene, mesh, meshes[i].indices);
        fill_texture_paths(scene, mesh, meshes[i]);
    }
}

void ModelLoader::fill_vertices(const aiScene *scene, const aiMesh *mesh, std::vector<Vertex> &vertices) {
    vertices.resize(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        if (mesh->HasPositions()) { // quite useless (?)
            vertices[i].position = glm::vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            );
        }

        if (mesh->HasTextureCoords(0)) {
            vertices[i].texture_coordinate = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }

        if (mesh->HasNormals()) {
            vertices[i].normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }
    }
}

void ModelLoader::fill_indices(const aiScene *scene, const aiMesh *mesh, std::vector<unsigned int> &indices) {
    indices.reserve(mesh->mNumFaces * 3);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int e = 0; e < face.mNumIndices; e++)
            indices.push_back(face.mIndices[e]);
    }
}

void ModelLoader::fill_texture_paths(const aiScene *scene, const aiMesh *mesh, MeshData &data) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    load_material_textures(material, aiTextureType_DIFFUSE, data.texture_paths.diffuse);
    load_material_textures(material, aiTextureType_SPECULAR, data.texture_paths.specular);
    load_material_textures(material, aiTextureType_EMISSIVE, data.texture_paths.emission);
}

void ModelLoader::load_material_textures(aiMaterial *material, aiTextureType type,
                                         std::vector<std::string> &texture_paths) {
    texture_paths.resize(material->GetTextureCount(type));
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString path;
        material->GetTexture(type, i, &path);
        texture_paths[i] = directory + "/" + path.C_Str();
    }
}