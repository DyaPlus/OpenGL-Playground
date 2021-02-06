#include "ScreenQuad.h"

unsigned int ScreenQuad::m_QuadVAO = 0;

void ScreenQuad::Init()
{
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,0,  0.0f, 1.0f,
        -1.0f, -1.0f,0,  0.0f, 0.0f,
         1.0f, -1.0f,0,  1.0f, 0.0f,

        -1.0f,  1.0f,0,  0.0f, 1.0f,
         1.0f, -1.0f,0,  1.0f, 0.0f,
         1.0f,  1.0f,0,  1.0f, 1.0f
    };
    // setup plane VAO
    glGenVertexArrays(1, &m_QuadVAO);
    unsigned int quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(m_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void ScreenQuad::Render()
{
    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
