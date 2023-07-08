//
// Created by cedric on 7/7/23.
//

#include "texture.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>

#include <stb/stb_image.h>
Texture::Texture()
{

}

bool Texture::load(const char *filename) {
    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    uint8_t* data = stbi_load(filename, &width, &height, &channels, 0);
    if(!data)
    {
        std::cerr << "Failed to open: " << filename << std::endl;
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return true;
}

void Texture::use()
{
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
}
uint32_t Texture::get_id() const
{
    return this->texture_id;
}

Texture::~Texture() {
    GLuint textures_to_delete[] = {this->texture_id};
    glDeleteTextures(1, textures_to_delete);
}