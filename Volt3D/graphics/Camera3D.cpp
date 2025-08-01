#include "Camera3D.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace Volt;

    
Camera3D::Camera3D(/* args */)
{
    position = {0.0f, 0.0f, 3.0f};
    yaw = -90.0f;
    pitch = 0.0f;
    fov = 70.0f;
    near_clip = 0.1f;
    far_clip = 100.0f;
        
    UP = {0.0f, 1.0f, 0.0f}; 
    FRONT = {0.0f, 0.0f, -1.0f};
    RIGHT = glm::normalize(glm::cross(FRONT, UP));

    speed = 5.0f;
    sensitivity = 0.1f;
}
    
Camera3D::~Camera3D()
{

}

void Camera3D::UpdateVectors()
{
    float yaw_rad = glm::radians(yaw);
    float pitch_rad = glm::radians(pitch);

    FRONT.x = cos(yaw_rad) * cos(pitch_rad);
    FRONT.y = sin(pitch_rad);
    FRONT.z = sin(yaw_rad) * cos(pitch_rad);
    FRONT = glm::normalize(FRONT);

    RIGHT = glm::normalize(glm::cross(FRONT, Vector3(0, 1, 0)));
    UP = glm::normalize(glm::cross(RIGHT, FRONT));
}

Matrix4 Camera3D::GetViewMatrix() const
{
    return glm::lookAt(position, position + FRONT, UP);
}

Matrix4 Camera3D::GetProjection(float aspect) const
{
    return glm::perspective(glm::radians(fov), aspect, near_clip, far_clip);
}