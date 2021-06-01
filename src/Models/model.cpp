#include <glad/glad.h>

#include "model.hpp"

Model::Model()
{
    glGenVertexArrays(1, &VAO);
}

void Model::makeModelTreeList()
{
}
