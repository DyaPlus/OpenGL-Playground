#ifndef MODEL_H
#define MODEL_H

#include "string"
#include "vector"
#include "Vertex.h"
#include "Shader.h"
#include "Mesh.h"

#include "GUIUtilities.h"
#include "vendor/imgui/imgui.h"

#define ASSIMP_DLL
#include "vendor/assimp/Importer.hpp"
#include "vendor/assimp/cimport.h"

#include "vendor/assimp/scene.h"
#include "vendor/assimp/postprocess.h"


class Model
{
protected:
    // model data
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;
    std::string m_Name;
    std::vector<Texture2D*> m_TexturesLoaded;
    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture2D*> loadMaterialTextures(aiMaterial* mat, aiTextureType type,TextureType TexType);
public:
    //TODO : find a better way to pass
    std::vector<Material*> m_MaterialsCreated;//For adding already created models to the active scene the model was added into
    Model(std::string path,std::string name);
    Model(std::string name); //Create A Custom Shape
    void EnableInstancing();

    //Public Methods
    void OnGuiUpdate(int id, int* selected);
    void Render();
    void DrawWithShader(Shader* shader);
    void SetPosition(glm::vec3 trans);
    void SetScale(glm::vec3 scale);
    void SetRotation(glm::vec3 rotation);
    void SetMaterial(Material* mat);
    std::string GetName();
    //Utilities
    unsigned int GetNumMeshes() const;
    Mesh GetMesh(unsigned int number) const;
    ~Model();
};

#endif