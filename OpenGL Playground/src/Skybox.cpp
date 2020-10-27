#include "Skybox.h"
#include "CubeVertices.h"

Skybox::Skybox(CubeMap* cube_map, Shader* shader)
	:m_CubeMap(cube_map),m_ShaderToUse(shader)
{
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    m_ID = VAO;

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), &get_skybox_data()[0], GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    m_ID = VAO;
    m_ShaderToUse->SetInteger("skybox", 11);
}

void Skybox::Render()
{
    glBindVertexArray(m_ID);
    m_ShaderToUse->Bind();
    glActiveTexture(GL_TEXTURE11); // We set the skybox to be 11
    m_CubeMap->Bind();
    glDepthMask(GL_FALSE);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_CubeMap->Unbind();
    glBindVertexArray(0);
    m_ShaderToUse->Unbind();
    glDepthMask(GL_TRUE);

}
