#pragma once

#include "Common.hpp"

namespace Volt
{
    class Camera3D
    {
    public:
        Camera3D(/* args */);
        ~Camera3D();

        void UpdateVectors();
        Matrix4 GetViewMatrix() const;
        Matrix4 GetProjection(float aspect) const; 

        Vector3 position;
        float yaw;
        float pitch;
        float fov;
        float near_clip;
        float far_clip;
        
        Vector3 UP;
        Vector3 FRONT;
        Vector3 RIGHT;

        float speed;
        float sensitivity;


    };

    
} // namespace Volt
