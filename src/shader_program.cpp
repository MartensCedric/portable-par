//
// Created by cedric on 7/7/23.
//
#include "shader_program.h"
#include <GL/glew.h>
#include <iostream>

ShaderProgram::ShaderProgram(Shader* vShader, Shader* fShader)
{
    this->id = glCreateProgram();
    glAttachShader(vShader->get_shader_id());
    glAttachShader(fShader->get_shader_id());
    glLinkProgram(this->id);

    int success;
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success) {
        char log[512];
        glGetProgramInfoLog(this->id, 512, nullptr, log);
        std::cerr << "Failed to link shader program: " << std::endl;
    }
}

uint32_t ShaderProgram::get_id() const {
    return this->id;
}

void ShaderProgram::use() {
    glUseProgram(this->id);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(this->id);
}
