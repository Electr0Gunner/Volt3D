#include "texture.hpp"

using namespace Smith;

    
Texture::Texture(int tex_width, int tex_height, int channel_number,unsigned char* pixel_data): 
width(tex_width), 
height(tex_height), 
channel_count(channel_number),
filter_type(TextureFilterType::FILTER_LINEAR),
pixels(pixel_data, pixel_data + (tex_width * tex_height * channel_number)), 
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
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    GLenum filtering = filter_type == TextureFilterType::FILTER_LINEAR ? GL_LINEAR : GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering); // Minification filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering); // Magnification filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format = (channel_count == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels.data());
}

void Texture::ClearGPUData()
{
    if (ID == 0)
        return;
    glDeleteTextures(1, &ID);
}