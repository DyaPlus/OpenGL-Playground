#include "Mesh.h"

Mesh::Mesh(float* vertices,int length, glm::vec3 pos, Shader* shader)
{
    m_Vertices = vertices;
	m_Pos = pos;
	m_ShaderToUse = shader;
    m_Color = glm::vec3(1.0, 0.0, 0.0);
    m_Material = new MaterialMap();

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*length, m_Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    m_ID = VAO;
}

void Mesh::SetMat(MaterialMap* mat)
{
    m_Material = mat;
    m_ShaderToUse->SetInteger("material.diffuse", 0);
    m_ShaderToUse->SetInteger("material.specular", 1);
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

    if (m_ShaderToUse->GetType() == ShaderType::Basic)
    {
        //TODO : Texture stuff clean them with no opengl calls
        if (m_Material->m_IsMapped)
        {
            glActiveTexture(GL_TEXTURE0);
            m_Material->m_DiffuseMap->Bind();
            glActiveTexture(GL_TEXTURE1);
            m_Material->m_SpecularMap->Bind();
        }
        else
        {
            m_ShaderToUse->SetVector3("material.diffuse", m_Material->m_Diffuse);
            m_ShaderToUse->SetVector3("material.specular", m_Material->m_Specular);
        }

        m_ShaderToUse->SetFloat("material.shininess", m_Material->m_Shininess);
    }
    
    m_ShaderToUse->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    m_ShaderToUse->Unbind();
}

Mesh::~Mesh()
{
    //delete m_Material; //TODO lol yala enta btems7 7aga gaya men bara
}
