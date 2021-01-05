#ifndef MATERIAL_H
#define MATERIAL_H
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Texture2D.h"

class MaterialMap
{
public:
    Texture2D* m_DiffuseMap;
    glm::vec3 m_Diffuse;
    Texture2D* m_SpecularMap;
    glm::vec3 m_Specular;
    Texture2D* m_NormalMap;
    float m_Shininess;

    uint8_t m_IsMapped;

    MaterialMap(Texture2D* diffuse, Texture2D* specular, float shininess)
        :m_DiffuseMap(diffuse), m_SpecularMap(specular), m_Shininess(shininess) , m_Diffuse(glm::vec3(1.0)),m_Specular(glm::vec3(1.0))
    {
        m_IsMapped = 0b110;
    }
    MaterialMap(Texture2D* diffuse, Texture2D* specular, Texture2D* normal , float shininess)
        :m_DiffuseMap(diffuse), m_SpecularMap(specular), m_NormalMap(normal), m_Shininess(shininess), m_Diffuse(glm::vec3(1.0)), m_Specular(glm::vec3(1.0))
    {
        m_IsMapped = 0b111;
    }
    MaterialMap() //basic white color
        :m_Diffuse(glm::vec3(1.0)), m_Specular(glm::vec3(0)), m_Shininess(0)
    {
        m_IsMapped = 0b000;
    }
    
    void SetDiffuseMap(Texture2D* diffuse);
    void SetSpecularMap(Texture2D* specular);
    void SetNormalMap(Texture2D* normal);
    void SetKd(glm::vec3 diffuse);
    void SetKa(glm::vec3 specular);

    void RemoveDiffuseMap();
    void RemoveSpecularMap();
    void RemoveNormalMap();

};

#endif