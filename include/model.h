//
// Created by cedric on 7/7/23.
//

#ifndef GMTK_2023_MODEL_H
#define GMTK_2023_MODEL_H

#include <vector>
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model
{
public:
    Model(const char* filename);
    void render();
private:
    std::vector<Mesh> meshes;
    void process_mesh(aiMesh* mesh, const aiScene* scene);

};
#endif //GMTK_2023_MODEL_H
