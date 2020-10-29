#include "CubeMap.h"
#include "ImageLoader.h"

CubeMap::CubeMap(std::string directory, std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    m_ID = textureID;
    m_Directory = directory;

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string lol = (m_Directory + faces[i]);
        Image tex = load_skybox_image((m_Directory + faces[i]).c_str());
        if (tex.data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << m_Directory + faces[i] << std::endl;
        }
        free_image(tex);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CubeMap::Bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}
void CubeMap::Unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}