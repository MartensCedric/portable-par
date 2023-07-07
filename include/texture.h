//
// Created by cedric on 7/7/23.
//

#ifndef GMTK_2023_TEXTURE_H
#define GMTK_2023_TEXTURE_H

#include <stdint.h>

class Texture
{
private:
    uint32_t texture_id;
public:
    explicit Texture();
    bool load(const char* filename);
    void use();
    uint32_t get_id() const;
    ~Texture();
};

#endif //GMTK_2023_TEXTURE_H
