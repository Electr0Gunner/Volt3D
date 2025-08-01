#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <vector>

namespace Volt
{
    class Texture
    {
    private:
        GLuint ID; //OpenGL texture ID used in binding
        std::vector<uint32_t> pixels;// Raw CPU data used for uploading
    public:
        Texture(int tex_width, int tex_height, uint32_t* pixel_data);
        ~Texture();

        void Upload();
        void ClearGPUData();

        int width;
        int height;
    };
} // namespace Volt
