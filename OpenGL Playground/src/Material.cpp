#include "Material.h"

void MaterialMap::SetDiffuseMap(Texture2D* diffuse)
{
	m_DiffuseMap = diffuse;
	m_IsMapped |= 0b100;
}

void MaterialMap::SetSpecularMap(Texture2D* specular)
{
	m_SpecularMap = specular;
	m_IsMapped |= 0b010;
}

void MaterialMap::SetNormalMap(Texture2D* normal)
{
	m_NormalMap = normal;
	m_IsMapped |= 0b001;
}

void MaterialMap::SetKd(glm::vec3 diffuse)
{
	m_Diffuse = diffuse;
}

void MaterialMap::SetKa(glm::vec3 specular)
{
	m_Specular = specular;
}

void MaterialMap::RemoveDiffuseMap()
{
	m_DiffuseMap = nullptr;
	m_IsMapped &= 0b011;
}

void MaterialMap::RemoveSpecularMap()
{
	m_SpecularMap = nullptr;
	m_IsMapped &= 0b101;
}

void MaterialMap::RemoveNormalMap()
{
	m_NormalMap = nullptr;
	m_IsMapped &= 0b110;
}
