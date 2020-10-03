#include "vendor/stb_image/stb_image.h"

struct Image
{
    int width, height, nrChannels;
    unsigned char* data;
    Image(unsigned char* data,int width, int height, int nrChannels)
        :data(data),width(width),height(height),nrChannels(nrChannels)
    {}
};

Image load_image(const char* filepath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    return Image(data, width, height, nrChannels);
}

void free_image(Image img)
{
    stbi_image_free(img.data);
}