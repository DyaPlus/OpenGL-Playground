#include "Mesh.h"

Mesh::Mesh(float* vertices,int length, glm::vec3 pos, Shader* shader)
{
    m_Vertices = vertices;
	m_Pos = pos;
	m_ShaderToUse = shader;

    m_Color = glm::vec3(1.0, 0.0, 0.0);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*length, m_Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    m_ID = VAO;
}

void Mesh::ChangePosition(glm::vec3 new_pos)
{
    m_Pos = new_pos;
}

glm::mat4 Mesh::ModelMat()
{
    return glm::translate(glm::mat4(1.0f),m_Pos);
}

void Mesh::Render()
{
    glBindVertexArray(m_ID);
    m_ShaderToUse->Bind();
    //TODO shader should handle the assignment
    //Change color in shader
    GLuint objectColorID = glGetUniformLocation(m_ShaderToUse->m_ID, "objectColor");
    glUniform3fv(objectColorID, 1, &m_Color[0]);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    m_ShaderToUse->Unbind();
}

Mesh::~Mesh()
{
   
}
