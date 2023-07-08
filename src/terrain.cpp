//
// Created by cedric on 7/8/23.
//

#include "terrain.h"

#include <iostream>
#include <stb/stb_image.h>

Terrain::Terrain(const char* height_file, const char* gradient_file)
{

    if(!load_height(height_file))
    {
        std::cerr << "Failed to open height: " << height_file << std::endl;
    }

    if(!load_gradient(gradient_file))
    {
        std::cerr << "Failed to open gradient: " << gradient_file << std::endl;
    }
}

bool Terrain::load_height(const char *height_file) {

    stbi_set_flip_vertically_on_load(false);
    int width, height, channels;
    uint8_t* data = stbi_load(height_file, &width, &height, &channels, 0);
    if(!data) return false;

    for(int row = 0; row < height; row++)
    {
        std::vector<float> one_row;
        for(int col = 0; col < width; col++)
        {
            uint8_t value = data[(row * width + col)];
            float normalized = static_cast<float>(value) / 255.0f;
            one_row.push_back(normalized - 0.5f);
        }
        heights.push_back(one_row);
    }

    return true;
}

bool Terrain::load_gradient(const char *gradient_file) {
    stbi_set_flip_vertically_on_load(false);
    int width, height, channels;
    uint8_t* data = stbi_load(gradient_file, &width, &height, &channels, 0);
    if(!data) return false;

    for(int row = 0; row < height; row++)
    {
        std::vector<glm::vec3> one_row;
        for(int col = 0; col < width; col++)
        {
            uint8_t* value = &data[(row * width + col)];
            uint8_t green = value[1];
            uint8_t blue = value[2];

            float orientation = 2.0f * M_PI * static_cast<float>(green) / 255.0f;
            float magnitude = static_cast<float>(blue) / 255.f;
            glm::vec3 vec = glm::vec3(glm::cos(orientation), 0.f, glm::sin(orientation)) * magnitude;
            one_row.push_back(vec);
        }
        gradients.push_back(one_row);
    }

    return true;
}

glm::vec3 Terrain::get_gradient(float x, float z)
{
    x += 10.f;
    z += 10.f;

    x /= 20.f;
    z /= 20.f;

    x *= 100.f;
    z *= 100.f;

    int col = std::floor(x);
    int row = std::floor(z);

    return gradients[row][col];
}

float Terrain::get_height(float x, float z)
{
    x += 10.f;
    z += 10.f;

    x /= 20.f;
    z /= 20.f;

    x *= 100.f;
    z *= 100.f;

    int col = std::floor(x);
    int row = std::floor(z);

    return heights[row][col];
}