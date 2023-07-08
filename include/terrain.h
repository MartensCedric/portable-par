//
// Created by cedric on 7/8/23.
//

#ifndef GMTK_2023_TERRAIN_H
#define GMTK_2023_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>
class Terrain
{
private:
    std::vector<std::vector<glm::vec3>> gradients;
    std::vector<std::vector<float>> heights;

    bool load_height(const char* height_file);
    bool load_gradient(const char* gradient_file);
public:
    explicit Terrain(const char* height_file, const char* gradient_file);
    glm::vec3 get_gradient(float x, float z);
    float get_height(float x, float z);
};

#endif //GMTK_2023_TERRAIN_H
