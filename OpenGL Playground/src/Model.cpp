#include "Model.h"
#include "CubeVertices.h"

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs ); //Tringulate the meshes + flip UVs for opengl 

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
    MaterialMap* mat;
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture2D*> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        if (diffuseMaps.size() == 0)
        {
            Texture2D Ceramic("res\\Tiles_035_basecolor.jpg", TextureType::DIFFUSE);
            diffuseMaps.push_back(new Texture2D(Ceramic));
        }
        //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture2D*> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, TextureType::SPECULAR);
        if (specularMaps.size() == 0)
        {
            Texture2D CeramicSpec("res\\Tiles_035_roughness.jpg", TextureType::SPECULAR);
            specularMaps.push_back(new Texture2D(CeramicSpec));
        }
        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        mat = new MaterialMap(diffuseMaps[0], specularMaps[0], 32.0f); //TODO shininess should be choosable
    }
    else //Default textures
    {
        Texture2D* Ceramic = new Texture2D("res\\Tiles_035_basecolor.jpg", TextureType::DIFFUSE);
        Texture2D* CeramicSpec = new Texture2D("res\\Tiles_035_roughness.jpg", TextureType::SPECULAR);
        
        mat = new MaterialMap(Ceramic, CeramicSpec, 1.0f);
    }
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

Model::Model(std::string path)
{
    loadModel(path);
}

Model::Model()
{
    Texture2D* Ceramic = new Texture2D("res\\Tiles_035_basecolor.jpg", TextureType::DIFFUSE);
    Texture2D* CeramicSpec = new Texture2D("res\\Tiles_035_roughness.jpg", TextureType::SPECULAR);
    MaterialMap* mat = new MaterialMap(Ceramic, CeramicSpec, 1.0f);

    Mesh cube(get_vertices_v(), mat);
    m_Meshes.push_back(cube);
}

void Model::Render()
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i].Render();
    }
}

void Model::SetPosition(glm::vec3 trans)
{
    for (auto& mesh : m_Meshes)
    {
        mesh.SetPosition(trans);
    }
}

void Model::SetScale(glm::vec3 scale)
{
    for (auto& mesh : m_Meshes)
    {
        mesh.SetScale(scale);
    }
}

unsigned int Model::GetNumMeshes() const
{
    return m_Meshes.size();
}

void Model::SetShader(Shader* shader)
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i].SetShader(shader);
    }
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
