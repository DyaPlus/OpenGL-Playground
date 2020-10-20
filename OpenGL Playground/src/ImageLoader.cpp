#include "ImageLoader.h"

Image load_image(const char* filepath)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    return Image(data, width, height, nrChannels);
}

Image load_skybox_image(const char* filepath)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    return Image(data, width, height, nrChannels);
}

void free_image(Image img)
{
    stbi_image_free(img.data);
}
