//
// Created by cedric on 7/7/23.
//

#include "model.h"

#include <iostream>

Model::Model(const char *filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Failed to load: " << filename << ", " << importer.GetErrorString() << std::endl;
    }

    aiMesh* mesh = scene->mMeshes[0];

    process_mesh(mesh, scene);
}

void Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<vertex_t> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture*> textures;

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        vertex_t v;
        v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0])
        {
            v.tex_coord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            v.tex_coord = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(v);
    }

    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(uint32_t j = 0; j < face.mNumIndices; i++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
}

void Model::render() {
    for(auto& m : meshes)
        m.render();
}