//
// Created by cedric on 7/7/23.
//

#include "mesh.h"
#include "texture.h"
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(const std::vector<vertex_t>& vertices, const std::vector<uint32_t>& indices,  const std::vector<Texture*>& textures)
: vertices(vertices), indices(indices), textures(textures)
{
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_t), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(offsetof(vertex_t, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(offsetof(vertex_t, tex_coord)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::render(ShaderProgram* shader_program)
{
    shader_program->use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}