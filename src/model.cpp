//
// Created by cedric on 7/7/23.
//

#include "model.h"
#include "shader_program.h"

#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

Model::Model(const char *filename) {
    Assimp::Importer importer;

    model = glm::mat4(1.0f);
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
        for(uint32_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    meshes.emplace_back(vertices, indices, textures);
}

void Model::render(ShaderProgram* shader) {

    int model_location = glGetUniformLocation(shader->get_id(), "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

    for(auto& m : meshes)
        m.render(shader);
}

void Model::render() {
    if(this->shader_program == nullptr)
        throw std::runtime_error("No shader associated with model!");
    render(this->shader_program);
}

void Model::set_shader(ShaderProgram* shader)
{
    this->shader_program = shader;
}

ShaderProgram *Model::get_shader() const {
    return shader_program;
}
