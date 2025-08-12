#include "Camera3D.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace Smith;

    
Camera3D::Camera3D(/* args */)
{
    position = {0.0f, 0.0f, 0.0f};
    yaw = -90.0f;
    pitch = 0.0f;
    fov = 70.0f;
    zoom = 1.0f;
    near_clip = 0.001f;
    far_clip = 10000.0f;
        
    UP = {0.0f, 1.0f, 0.0f}; 
    FRONT = {0.0f, 0.0f, -1.0f};
    RIGHT = glm::normalize(glm::cross(FRONT, UP));

    view_mode = CameraViewMode::VIEW_PERSPECTIVE;
}
    
Camera3D::~Camera3D()
{
    glDeleteFramebuffers(1, &render_target.FBO);
    glDeleteTextures(1, &render_target.color_texture);
    glDeleteRenderbuffers(1, &render_target.depth_buffer);
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

Matrix4 Camera3D::GetProjection() const
{
    float aspect = (float)render_target.viewport_rect.width / (float)render_target.viewport_rect.height;
    if (view_mode == CameraViewMode::VIEW_PERSPECTIVE)
    {
        return glm::perspective(glm::radians(fov), aspect, near_clip, far_clip);
    }
    else
    {
        float half_w = zoom * aspect * 0.5f;
        float half_h = zoom * 0.5f;
        return glm::ortho(-half_w, half_w, -half_h, half_h, near_clip, far_clip);
    }
}

#include <glm/gtc/matrix_inverse.hpp>

Vector3 Camera3D::ScreenToWorldRay(const Vector2& screen_pos, float screen_width, float screen_height) const
{
    float x = (2.0f * screen_pos.x) / screen_width - 1.0f;
    float y = 1.0f - (2.0f * screen_pos.y) / screen_height;
    glm::vec4 ray_clip(x, y, -1.0f, 1.0f);

    Matrix4 proj = GetProjection();
    glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    Matrix4 view = GetViewMatrix();
    glm::vec4 ray_world = glm::inverse(view) * ray_eye;
    Vector3 dir = glm::normalize(Vector3(ray_world));

    return dir;
}

Vector3 Camera3D::ScreenToWorldPoint(const Vector2& screen_pos, float screen_width, float screen_height, float depth) const
{
    Vector3 ray_dir = ScreenToWorldRay(screen_pos, screen_width, screen_height);
    return position + ray_dir * depth;
}
