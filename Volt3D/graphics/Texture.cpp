#include "texture.hpp"

using namespace Volt;

    
Texture::Texture(int tex_width, int tex_height, uint32_t* pixel_data): 
width(tex_width), 
height(tex_height), 
pixels(pixel_data, pixel_data + (tex_width * tex_height)), 
ID(0)
{
}
    
Texture::~Texture()
{
    ClearGPUData();
}
    
void Texture::Upload()
{
    ClearGPUData();

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Minification filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Magnification filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}

void Texture::ClearGPUData()
{
    if (ID == 0)
        return;
    glDeleteTextures(1, &ID);
}