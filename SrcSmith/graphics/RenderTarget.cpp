#include "RenderTarget.hpp"
#include "Renderer3D.hpp"

using namespace Smith;

RenderTarget::RenderTarget()
{
    color_texture = 0;
    depth_buffer = 0;
    FBO = 0;
}

RenderTarget::RenderTarget(RenderTarget&& other) noexcept
{
    FBO = other.FBO;
    color_texture = other.color_texture;
    depth_buffer = other.depth_buffer;
    type = other.type;
    render_type = other.render_type;
    viewport_rect = other.viewport_rect;

    other.FBO = 0;
    other.color_texture = 0;
    other.depth_buffer = 0;
}

RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept
{
    if (this != &other)
    {
        if (color_texture != 0)
            glDeleteTextures(1, &color_texture);
        if (depth_buffer != 0)
            glDeleteRenderbuffers(1, &depth_buffer);
        if (FBO != 0)
            glDeleteFramebuffers(1, &FBO);
            
        FBO = other.FBO;
        color_texture = other.color_texture;
        depth_buffer = other.depth_buffer;
        type = other.type;
        render_type = other.render_type;
        viewport_rect = other.viewport_rect;

        other.FBO = 0;
        other.color_texture = 0;
        other.depth_buffer = 0;
    }
    return *this;
}


RenderTarget::~RenderTarget()
{
    if (color_texture != 0)
        glDeleteTextures(1, &color_texture);

    if (depth_buffer != 0)
        glDeleteRenderbuffers(1, &depth_buffer);

    if (FBO != 0)
        glDeleteFramebuffers(1, &FBO);
}

RenderTarget RenderTarget::CreateTarget(Renderer3D* renderer, int width, int height)
{
    RenderTarget target;
    target.type = TargetType::TARGET_SCREEN;
    target.viewport_rect = renderer->GetViewport(target, width, height); //target is placed there just for the argument, cause it wont actually do anything with it

    glGenTextures(1, &target.color_texture);
    glBindTexture(GL_TEXTURE_2D, target.color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, target.viewport_rect.width, target.viewport_rect.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &target.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, target.FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target.color_texture, 0);

    glGenRenderbuffers(1, &target.depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, target.depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, target.viewport_rect.width, target.viewport_rect.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, target.depth_buffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return target; 
}