#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialMap* material)
{
    m_Vertices = vertices;
    m_Indices = indices;

	m_Pos = glm::vec3(0.0f);
	m_Material = material;

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex) , &m_Vertices[0], GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,m_Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_TexCoords));
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

void Mesh::SetShader(Shader* shader)
{
    m_ShaderToUse = shader;
}

void Mesh::SetPosition(glm::vec3 new_pos)
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
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    m_ShaderToUse->Unbind();
}

Mesh::~Mesh()
{
    //delete m_Material; //TODO lol yala enta btems7 7aga gaya men bara
}
