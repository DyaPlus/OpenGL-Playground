#ifndef MATERIAL_H
#define MATERIAL_H
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Texture2D.h"

struct Material
{
    glm::vec3 m_Ambient;
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;
    float m_Shininess;
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
        :m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Shininess(shininess)
    {

    }
    Material() //basic white color
        :m_Ambient(glm::vec3(0)), m_Diffuse(glm::vec3(1.0)), m_Specular(glm::vec3(0)), m_Shininess(0)
    {

    }
}; 

struct MaterialMap
{
    glm::vec3 m_Ambient;
    Texture2D* m_DiffuseMap;
    glm::vec3 m_Diffuse;
    Texture2D* m_SpecularMap;
    glm::vec3 m_Specular;
    float m_Shininess;

    bool m_IsMapped;

    MaterialMap(glm::vec3 ambient, Texture2D* diffuse, Texture2D* specular, float shininess)
        :m_Ambient(ambient), m_DiffuseMap(diffuse), m_SpecularMap(specular), m_Shininess(shininess)
    {
        m_IsMapped = true;
    }
    MaterialMap() //basic white color
        :m_Ambient(glm::vec3(0)), m_Diffuse(glm::vec3(1.0)), m_Specular(glm::vec3(0)), m_Shininess(0)
    {
        m_IsMapped = false;

    }
};

#endif