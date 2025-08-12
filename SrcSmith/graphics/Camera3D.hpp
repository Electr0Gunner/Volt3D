#pragma once

#include "Common.hpp"
#include "RenderTarget.hpp"

namespace Smith
{
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
        Matrix4 GetProjection() const; 
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
