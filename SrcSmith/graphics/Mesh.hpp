#pragma once

#include "Common.hpp"
#include "VertexObject.hpp"
#include "Shader.hpp"

#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Smith
{
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        ~Mesh();

        void Draw();

        std::unique_ptr<Shader> shader;
        std::unique_ptr<VertexObject> geometry;
        Matrix4 transform;
        GLuint texture_ID;
    };

    
} // namespace Smith
