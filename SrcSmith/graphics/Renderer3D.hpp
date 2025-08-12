#pragma once

#include "Common.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera3D.hpp"

#include <vector>

#include <SDL3/SDL_video.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Smith
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
        Renderer3D();
        ~Renderer3D();

        virtual RendererInitCode Init();
        virtual void Cleanup();

        virtual void BeginFrame();
        virtual void EndFrame();
        virtual void Render();
        virtual void SendCommand(RenderCommand cmd);
        virtual Rect GetViewport(const RenderTarget& target, int logical_width, int logical_height);
        virtual void UpdateViewport(RenderTarget& target, int logical_width, int logical_height);
        virtual RenderTarget CreateRenderTarget();

        virtual std::string GetErrorStr();

        SDL_GLContext GetContext()
        {
            return context;
        }

    };
} // namespace Smith
