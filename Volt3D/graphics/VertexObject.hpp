#pragma once

#include "Common.hpp"

#include <vector>

#include <glad/glad.h>

namespace Volt
{

    struct Vertex {
        Vector3 position;
        Vector2 uv;
        Vector4 color;
    };

    class VertexObject
    {
    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        bool has_uploaded;

        GLuint VAO;
        GLuint VBO;
        GLuint IBO;
    public:
        VertexObject();
        ~VertexObject();

        void Upload();
        void EnsureUpload();
        void SetVertices(const std::vector<Vertex>& new_vertices);
        void SetIndices(const std::vector<uint32_t>& new_indices);

        void Draw(GLenum primitive);
    };
} // namespace Volt
