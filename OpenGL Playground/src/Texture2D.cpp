#include "Texture2D.h"
#include "ImageLoader.h"

Texture2D::Texture2D(const char* filepath)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    m_ID = texture;
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    //TODO Change by additional methods
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    Image tex = load_image(filepath);

    if (tex.data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    free_image(tex);
}

void Texture2D::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
}
void Texture2D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
