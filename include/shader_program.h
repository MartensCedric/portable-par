//
// Created by cedric on 7/7/23.
//

#ifndef GMTK_2023_SHADER_PROGRAM_H
#define GMTK_2023_SHADER_PROGRAM_H

#include "shader.h"
#include <stdint.h>

class ShaderProgram
{
private:
    uint32_t id;
public:
    explicit ShaderProgram(Shader* vShader, Shader* fShader);
    uint32_t get_id() const;
    void use();
    ~ShaderProgram();
};
#endif //GMTK_2023_SHADER_PROGRAM_H
