#include "Shapes.h"

#include "CubeVertices.h"

Cube::Cube(std::string name,unsigned int num_of_instances)
    : Model(name)
{
    Texture2D* DefaultTex = new Texture2D("res/default/texture/def_COLOR.jpg", TextureType::DIFFUSE);
    Texture2D* DefaultTexSpec = new Texture2D("res/default/texture/def_SPECULAR.jpg", TextureType::SPECULAR);
    Texture2D* DefaultTexNormal = new Texture2D("res/default/texture/def_Normal.jpg", TextureType::NORMAL);
    Material* mat = new Material();
    mat->SetDiffuseMap(DefaultTex);
    mat->SetNormalMap(DefaultTexNormal);
    mat->SetKs(glm::vec3(1));
    mat->SetShininess(128.0f);
    m_MaterialsCreated.push_back(mat); //TODO Model class shouldnt hold info of the material or create it

    Mesh cube(get_vertices_v(), mat,num_of_instances);
    m_Meshes.push_back(cube);
}

void Cube::SetInstancingData(glm::vec3* data, unsigned int num_of_instances)
{
    for (auto& mesh : m_Meshes)
    {
        mesh.SetInstancingData(data, num_of_instances);
    }
}

