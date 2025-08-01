#pragma once

#include "Common.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

#include <vector>

#include <SDL3/SDL_video.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Volt
{
    struct RenderCommand {
        Shader* shader;
        VertexObject* vertex_data;
        Matrix4 model;
        GLuint texture;
        GLenum primitive_type;
    };

    enum RendererInitCode
    {
        CODE_RENDERER_OK,
        CODE_RENDERER_FAIL
    };

    class Renderer3D
    {
    private:
        virtual void Flush();

        SDL_GLContext context;
        SDL_Window* window_ptr;

        std::vector<RenderCommand> command_buffer;

        Shader* DEFAULT_SHADER;
        Mesh* TEST_MESH;
    public:
        Renderer3D(int test);
        ~Renderer3D();

        virtual RendererInitCode Init();
        virtual void Cleanup();

        virtual void BeginFrame();
        virtual void EndFrame();
        virtual void Render();
        virtual void SendCommand(RenderCommand cmd);

        virtual std::string GetErrStr();

        SDL_GLContext GetContext()
        {
            return context;
        }
    };
} // namespace Volt
