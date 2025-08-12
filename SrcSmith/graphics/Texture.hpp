#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <vector>

namespace Smith
{
    enum TextureFilterType
    {
        FILTER_NEAREST,
        FILTER_LINEAR,
    };

    class Texture
    {
    private:
        GLuint ID; //OpenGL texture ID used in binding
        std::vector<unsigned char> pixels;// Raw CPU data used for uploading
    public:
        Texture(int tex_width, int tex_height, int channel_number, unsigned char* pixel_data);
        ~Texture();

        void Upload();
        void ClearGPUData();
        GLuint GetID() 
        {
            if (ID == 0)
                Upload();
            return ID;
        }

        int width;
        int height;
        int channel_count;
        TextureFilterType filter_type;
    };
} // namespace Smith
