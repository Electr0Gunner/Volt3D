#pragma once

#include "Common.hpp"
#include "math/Rect.hpp"
#include <glad/glad.h>

namespace Smith
{

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

    struct RenderTarget
    {
        TargetType type = TargetType::TARGET_SCREEN;
        RenderType render_type = RenderType::RENDER_TEXTURED;
        GLuint FBO;
        GLuint color_texture;
        GLuint depth_buffer;
        Rect viewport_rect;

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
    };

    enum CameraViewMode
    {
        VIEW_PERSPECTIVE,
        VIEW_ORTOGRAGHIC,
    };

    class Camera3D
    {
    public:
        Camera3D(/* args */);
        ~Camera3D();

        void UpdateVectors();
        Matrix4 GetViewMatrix() const;
        Matrix4 GetProjection(float aspect) const; 
        Vector3 ScreenToWorldRay(const Vector2& screen_pos, float screen_width, float screen_height) const;
        Vector3 ScreenToWorldPoint(const Vector2& screen_pos, float screen_width, float screen_height, float depth) const;


        Vector3 position;
        float yaw;
        float pitch;
        float fov;
        float zoom;
        float near_clip;
        float far_clip;
        
        Vector3 UP;
        Vector3 FRONT;
        Vector3 RIGHT;

        CameraViewMode view_mode;

        RenderTarget render_target;

    };

    
} // namespace Smith
