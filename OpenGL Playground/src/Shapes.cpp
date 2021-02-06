#include "Shapes.h"

#include "CubeVertices.h"

Cube::Cube(std::string name)
    : Model(name)
{
    Texture2D* DefaultTex = new Texture2D("res/default/texture/def2_COLOR.jpg", TextureType::DIFFUSE);
    Texture2D* DefaultTexSpec = new Texture2D("res/default/texture/def_SPECULAR.jpg", TextureType::SPECULAR);
    Texture2D* DefaultTexNormal = new Texture2D("res/default/texture/def2_Normal.jpg", TextureType::NORMAL);
    Material* mat = new Material();
    mat->SetDiffuseMap(DefaultTex);
    mat->SetNormalMap(DefaultTexNormal);
    mat->SetKs(glm::vec3(1));
    mat->SetShininess(128.0f);
    m_MaterialsCreated.push_back(mat); //TODO Model class shouldnt hold info of the material or create it

    Mesh cube(get_vertices_v(), mat);
    m_Meshes.push_back(cube);
}
