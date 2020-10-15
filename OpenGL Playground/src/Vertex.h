#ifndef VERTEX_H
#define VERTEX_H

#include "vendor/glm/gtc/matrix_transform.hpp"

struct Vertex
{
    glm::vec3 m_Position;
    glm::vec3 m_Normal;
    glm::vec2 m_TexCoords;
};

#endif
