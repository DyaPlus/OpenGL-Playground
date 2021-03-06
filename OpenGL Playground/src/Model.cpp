#include "Model.h"

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace ); //Tringulate the meshes + flip UVs for opengl + Calculate tangents for normal mapping

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    m_Directory = path.substr(0, path.find_last_of('/')); //Save the model driectory when loaded for further updates

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        m_Meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        vertex.m_Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.m_Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.m_Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        //TODO Precalculate the bittangent
        if (mesh->mTextureCoords[0])
        {
            vertex.m_TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.m_TexCoords = glm::vec2(0.0f);
        }
        vertices.push_back(vertex);
    }
    // process indices
    
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            indices.push_back(mesh->mFaces[i].mIndices[j]); 
        }
    }
   

    // process material
    //TODO : simplify or improve
    //TODO : handle default material if failed to load the required textures and remove the hack

    Material* mat;
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture2D*> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, TextureType::DIFFUSE);

        mat = new Material(); //TODO shininess should be choosable
        if (diffuseMaps.size() != 0) 
        {
            mat->SetDiffuseMap(diffuseMaps[0]);
        }
        //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture2D*> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, TextureType::SPECULAR);
        if (specularMaps.size() != 0)
        {
            mat->SetSpecularMap(specularMaps[0]);
        }
        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    else //Default Material
    {   
        mat = new Material();
    }
    

    m_MaterialsCreated.push_back(mat); //TODO Scene Object should be the one to record created materials
    return Mesh(vertices, indices, mat);
}

std::vector<Texture2D*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType TexType)
{
    //TODO : optimize if the texture was already loaded 
    std::vector<Texture2D*> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path_to_tex = m_Directory + "/" + std::string(str.C_Str());
        bool exist = false;
        for (int j = 0; j < m_TexturesLoaded.size(); ++j)
        {
            if (!std::strcmp(m_TexturesLoaded[j]->GetPath().c_str(), path_to_tex.c_str()))
            {
                std::cout << "found it" << std::endl;
                textures.push_back(m_TexturesLoaded[j]);
                exist = true;
                break;
            }
        }
        if (!exist)
        {
        std::cout << "Loading : " << path_to_tex << std::endl;
        Texture2D *texture = new Texture2D(path_to_tex.c_str(), TexType);
        textures.push_back(texture);
        m_TexturesLoaded.push_back(texture);
        }

    }
    return textures;
}

Model::Model(std::string path, std::string name)
{
    loadModel(path);
    m_Name = name;
}

Model::Model(std::string name)
    :m_Position(0,0,0), m_Scale(1, 1, 1), m_Rotation(0, 0, 0)
{
    m_Name = name; //TODO : we default it to cube as it is currently used for GUI Listing
}

void Model::EnableInstancing()
{
    for (auto& mesh : m_Meshes)
    {
        mesh.EnableInstancing();;
    }
}


void Model::OnGuiUpdate(int id,int* selected)
{
    ImGui::Begin("Entities");
    bool selection_status = id == *selected; // Used to impelement diselect functionality


    if (ImGui::Selectable(m_Name.c_str(), selection_status))
    {
        if(selection_status) //Deselect if already selected
        {
            *selected = -1;
        }
        else
        {
            *selected = id;
        }
    }
    ImGui::End();

    if (selection_status)
    {
        ImGui::Begin("Inspector");
        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::PushID(1);
            if (GUI_UT::InputFloat3("Position: ", &m_Position[0]));
            {
                SetPosition(m_Position);
            }
            ImGui::PopID();
            ImGui::PushID(2);
            if (GUI_UT::InputFloat3("Scale: ", &m_Scale[0]));
            {
                SetScale(m_Scale);
            }
            ImGui::PopID();
            ImGui::PushID(3);

            if (GUI_UT::InputFloat3("Rotation: ", &m_Rotation[0]));
            {
                SetRotation(m_Rotation);
            }
            ImGui::PopID();
        }


        ImGui::End();
    }
}

void Model::Render()
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i].Render();
    }
}

void Model::DrawWithShader(Shader* shader)
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i].DrawWithShader(shader);
    }
}

void Model::SetPosition(glm::vec3 trans)
{
    m_Position = trans;
    for (auto& mesh : m_Meshes)
    {
        mesh.SetPosition(trans);
    }
}

void Model::SetScale(glm::vec3 scale)
{
    m_Scale = scale;
    for (auto& mesh : m_Meshes)
    {
        mesh.SetScale(scale);
    }
}

void Model::SetRotation(glm::vec3 rotation)
{
    m_Rotation = rotation;

    for (auto& mesh : m_Meshes)
    {
        mesh.SetRotation(rotation);
    }
}

void Model::SetMaterial(Material* mat)
{
    m_MaterialsCreated.pop_back();
    m_MaterialsCreated.push_back(mat);
    for (auto& mesh : m_Meshes)
    {
        mesh.SetMat(mat);
    }
}

std::string Model::GetName()
{
	return m_Name;
}

unsigned int Model::GetNumMeshes() const
{
    return m_Meshes.size();
}


Mesh Model::GetMesh(unsigned int number) const
{
    return m_Meshes[number];
}

Model::~Model()
{
    for (int i = 0; i < m_TexturesLoaded.size(); ++i)
    {
        delete m_TexturesLoaded[i];
    }
}
