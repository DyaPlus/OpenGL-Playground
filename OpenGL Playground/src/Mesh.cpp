#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material)
{
    m_Indexed = true;
    m_Vertices = vertices;
    m_Indices = indices;

	m_Pos = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
    m_Rotation = glm::vec3(0.0f);
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,m_Normal)); //Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_TexCoords)); //UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Tangent)); //Tangent vector
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    m_ID = VAO;
}

Mesh::Mesh(std::vector<Vertex> vertices, Material* material)
{
    m_Indexed = false;
    m_Vertices = vertices;

    m_Pos = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
    m_Rotation = glm::vec3(0.0f);

    m_Material = material;
    
    CalculateTangents();

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Normal)); //Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_TexCoords)); //UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Tangent)); //Tangent vector
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    m_ID = VAO;
}

void Mesh::SetMat(Material* mat)
{
    m_Material = mat;
}

void Mesh::SetPosition(glm::vec3 new_pos)
{
    m_Pos = new_pos;
}

void Mesh::SetScale(glm::vec3 new_scale)
{
    m_Scale = new_scale;
}

void Mesh::SetRotation(glm::vec3 new_rot)
{
    new_rot = glm::vec3(glm::radians(new_rot.x), glm::radians(new_rot.y), glm::radians(new_rot.z));
    m_Rotation = new_rot;
}

void Mesh::CalculateTangents() //NOTE : Used for un indexed meshes only , as the the indexed should be handled by the 3rd party model loader
{
    if (!m_Indexed)
    {
        for (int i = 0; i < m_Vertices.size(); i = i + 3)
        {
            glm::vec3 tangent;
            //Calculate The edge vectors of the triangle
            glm::vec3 edge1 = m_Vertices[i + 1].m_Position - m_Vertices[i].m_Position;
            glm::vec3 edge2 = m_Vertices[i + 2].m_Position - m_Vertices[i].m_Position;

            //Calculate The UV difference of the edges of the triangle
            glm::vec2 deltaUV1 = m_Vertices[i + 1].m_TexCoords - m_Vertices[i].m_TexCoords;
            glm::vec2 deltaUV2 = m_Vertices[i + 2].m_TexCoords - m_Vertices[i].m_TexCoords;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            //TODO : Optimize
            m_Vertices[i].m_Tangent = tangent;
            m_Vertices[i+1].m_Tangent = tangent;
            m_Vertices[i+2].m_Tangent = tangent;
        }
    }
}

glm::mat4 Mesh::ModelMat()
{
    glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), m_Scale);
    glm::mat4 rotation_mat = glm::rotate(glm::mat4(1.0f), m_Rotation.x ,glm::vec3(1,0,0));
    rotation_mat = glm::rotate(rotation_mat, m_Rotation.y, glm::vec3(0, 1, 0));
    rotation_mat = glm::rotate(rotation_mat, m_Rotation.z, glm::vec3(0, 0, 1));
    glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), m_Pos);

    return trans_mat * rotation_mat * scale_mat;
}

void Mesh::Render()
{
    glBindVertexArray(m_ID);
    
    m_Material->BindValues();

    m_Material->m_ShaderToUse->Bind();
    if (m_Indexed)
    {
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    }

    glBindVertexArray(0);
    m_Material->m_ShaderToUse->Unbind();
    glActiveTexture(GL_TEXTURE0); //Reset default texture unit
}

void Mesh::DrawWithShader(Shader* shader)
{
    glBindVertexArray(m_ID);

    m_Material->BindValuesToShader(shader);

    shader->Bind();
    if (m_Indexed)
    {
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    }

    glBindVertexArray(0);
    shader->Unbind();
    glActiveTexture(GL_TEXTURE0); //Reset default texture unit
}

Mesh::~Mesh()
{
    //delete m_Material; //TODO lol yala enta btems7 7aga gaya men bara
}
