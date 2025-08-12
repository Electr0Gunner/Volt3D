#pragma once

#include "math/Rect.hpp"
#include <glad/glad.h>

namespace Smith
{
    class Renderer3D;

    enum TargetType
    {
        TARGET_SCREEN,
        TARGET_FBO,
    };

    enum RenderType
    {
        RENDER_TEXTURED,
        RENDER_NO_TEXTURE, 
        RENDER_WIREFRAME, 
    };


    class RenderTarget
    {
        public:
            TargetType type = TargetType::TARGET_SCREEN;
            RenderType render_type = RenderType::RENDER_TEXTURED;
            GLuint FBO;
            GLuint color_texture;
            GLuint depth_buffer;
            Rect viewport_rect;

            RenderTarget();
            ~RenderTarget();

            RenderTarget(const RenderTarget&) = delete;
            RenderTarget& operator=(const RenderTarget&) = delete;

            RenderTarget(RenderTarget&& other) noexcept;
            RenderTarget& operator=(RenderTarget&& other) noexcept;

            void Bind() const
            {
                glBindFramebuffer(GL_FRAMEBUFFER, IsOffscreen() ? FBO : 0);
                glViewport(viewport_rect.x, viewport_rect.y, viewport_rect.width, viewport_rect.height);
            }

            void Unbind() const
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            bool IsOffscreen() const
            {
                return type == TargetType::TARGET_FBO;
            }

            void ResizeTarget(Rect size)
            {
                viewport_rect = size;

                if (color_texture != 0)
                    glDeleteTextures(1, &color_texture);
                if (depth_buffer != 0)
                    glDeleteRenderbuffers(1, &depth_buffer);
                if (FBO != 0)
                    glDeleteFramebuffers(1, &FBO);

                glGenTextures(1, &color_texture);
                glBindTexture(GL_TEXTURE_2D, color_texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewport_rect.width, viewport_rect.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glGenFramebuffers(1, &FBO);
                glBindFramebuffer(GL_FRAMEBUFFER, FBO);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);

                glGenRenderbuffers(1, &depth_buffer);
                glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewport_rect.width, viewport_rect.height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            static RenderTarget CreateTarget(Renderer3D* renderer, int width, int height);
    };

} // namespace Smith
