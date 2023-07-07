//
// Created by cedric on 7/7/23.
//

#include "shader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <GL/glew.h>

Shader::Shader(const char *filename, enum shader_type shader_type) : filename(filename), type(shader_type) {
}


std::string Shader::read_file(const std::string& filename)
{
    std::string contents;
    std::ifstream shader_file(filename);

    if(shader_file.is_open())
    {
        contents = std::string(std::istreambuf_iterator<char>(shader_file), std::istreambuf_iterator<char>());
    }
    else
    {
        std::cerr << "Could not open file: " << filename << std::endl;
    }

    return contents;
}

bool Shader::compile() {

    std::string contents = read_file(this->filename);
    GLenum gl_shader_type;
    switch(type)
    {
        case vertex:
            gl_shader_type = GL_VERTEX_SHADER;
            break;
        case fragment:
            gl_shader_type = GL_FRAGMENT_SHADER;
            break;
    }

    this->shader_id = glCreateShader(gl_shader_type);
    const char* source_string = contents.c_str();
    glShaderSource(this->shader_id, 1, &source_string, nullptr);
    glCompileShader(this->shader_id);

    char log[512];
    int success;
    glGetShaderiv(this->shader_id, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(this->shader_id, 512, nullptr, log);
        std::cerr << this->filename << ": Shader compilation failed -> " << log << std::endl;
        return false;
    }

    return true;
}

Shader::~Shader() {
    glDeleteShader(this->shader_id);
}

u_int32_t Shader::get_shader_id() const {
    return this->shader_id;
}

