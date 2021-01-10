#include "Material.h"

void Material::SetShader(Shader* shader) //TODO : It is currently the only way to bind a shader to the material #LAME
{
    m_ShaderToUse = shader;
    shader->SetInteger("material.diffuse", 0);
    shader->SetInteger("material.specular", 1);
    shader->SetInteger("material.normal", 2);
}

void Material::SetDiffuseMap(Texture2D* diffuse)
{
	m_DiffuseMap = diffuse;
	m_IsMapped |= 0b100;
}

void Material::SetSpecularMap(Texture2D* specular)
{
	m_SpecularMap = specular;
	m_IsMapped |= 0b010;
}

void Material::SetNormalMap(Texture2D* normal)
{
	m_NormalMap = normal;
	m_IsMapped |= 0b001;
}

void Material::SetKd(glm::vec3 diffuse)
{
	m_Diffuse = diffuse;
}

void Material::SetKs(glm::vec3 specular)
{
	m_Specular = specular;
}

void Material::SetShininess(float shineness)
{
	m_Shininess = shineness;
}

void Material::RemoveDiffuseMap()
{
	m_DiffuseMap = nullptr;
	m_IsMapped &= 0b011;
}

void Material::RemoveSpecularMap()
{
	m_SpecularMap = nullptr;
	m_IsMapped &= 0b101;
}

void Material::RemoveNormalMap()
{
	m_NormalMap = nullptr;
	m_IsMapped &= 0b110;
}

void Material::BindValues()
{
   
    //TODO : Introduce better abstraction for specific shader code
    //The material assumes a Blinn Phong shader
    if (m_ShaderToUse->GetType() == ShaderType::Basic)
    {
        m_ShaderToUse->SetVector3("material.Kd", m_Diffuse);
        m_ShaderToUse->SetVector3("material.Ks", m_Specular);
        m_ShaderToUse->SetInteger("material.mapping", m_IsMapped);
        if (m_IsMapped & 0b100)
        {
            glActiveTexture(GL_TEXTURE0); //Activate 0 for diffuse
            m_DiffuseMap->Bind();
        }
        if (m_IsMapped & 0b010)
        {
            glActiveTexture(GL_TEXTURE1); //Activate 1 for diffuse
            m_SpecularMap->Bind();
        }
        if (m_IsMapped & 0b001)
        {
            glActiveTexture(GL_TEXTURE2); //Activate 2 for normal
            m_NormalMap->Bind();
        }
        m_ShaderToUse->SetFloat("material.shininess", m_Shininess);
    }
}
