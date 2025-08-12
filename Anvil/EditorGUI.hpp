#pragma once

#include <ImGui/ImGuiWindow.hpp>
#include "MapWriter.hpp"
#include <App.hpp>
#include <Common.hpp>
#include <Input.hpp>

namespace Anvil
{
    struct BrushCreationState
    {
        bool creating = false;
        Smith::Vector2 start_world;
        Smith::Vector2 current_world;
    };

    enum ViewWindow
    {
        VIEW_FRONT,
        VIEW_SIDE,
        VIEW_TOP,
        VIEW_NONE,
    };

    enum AnvilToolType
    {
        TOOL_SELECT,
        TOOL_CAMERA,
        TOOL_ACTOR,
        TOOL_BRUSH,
    };

    class EditorGUI : public Smith::ImGuiWindow
    {
    private:
        /* data */
    public:
        EditorGUI(/* args */);
        ~EditorGUI();

        virtual void Process();
        void SetupDockspace();
        void HandleBrushTool(Smith::Camera3D* camera);
        void UpdateCameraViewport(Smith::Camera3D* camera);
        bool setup_dockspace;
        Smith::Camera3D* main_camera;
        std::shared_ptr<Smith::Camera3D> top_camera;
        std::shared_ptr<Smith::Camera3D> side_camera;
        std::shared_ptr<Smith::Camera3D> front_camera;

        AnvilToolType current_tool;
        ViewWindow current_2D_moving_window;
        bool zoom_2d_view;

        //Camera stuff

        float cam_sensitivity = 3.0f;
        float cam_move_speed;
        bool freecam_active;
        bool panning_camera_active;
        bool camera_2D_movement;

        //
    };
    
    EditorGUI::EditorGUI(/* args */)
    {
        type_id = 1;
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        setup_dockspace = false;

        top_camera = std::make_shared<Smith::Camera3D>();
        side_camera = std::make_shared<Smith::Camera3D>();
        front_camera = std::make_shared<Smith::Camera3D>();

        Smith::Renderer3D* renderer_ptr = Smith::App::Instance()->renderer.get();

        top_camera->view_mode = Smith::CameraViewMode::VIEW_ORTOGRAGHIC;
        top_camera->render_target = Smith::RenderTarget::CreateTarget(renderer_ptr, 1600, 900);
        top_camera->render_target.render_type = Smith::RenderType::RENDER_WIREFRAME;
        top_camera->position = Smith::Vector3(0.0f, 5.0f, 0.0f);
        top_camera->yaw = 90.0f;
        top_camera->pitch = -90.0f;
        top_camera->UpdateVectors();

        side_camera->view_mode = Smith::CameraViewMode::VIEW_ORTOGRAGHIC;
        side_camera->render_target = Smith::RenderTarget::CreateTarget(renderer_ptr, 1600, 900);
        side_camera->render_target.render_type = Smith::RenderType::RENDER_WIREFRAME;
        side_camera->position = Smith::Vector3(5.0f, 0.0f, 0.0f);
        side_camera->yaw = 180.0f; 
        side_camera->pitch = 0.0f;
        side_camera->UpdateVectors();

        front_camera->view_mode = Smith::CameraViewMode::VIEW_ORTOGRAGHIC;
        front_camera->render_target = Smith::RenderTarget::CreateTarget(renderer_ptr, 1600, 900);
        front_camera->render_target.render_type = Smith::RenderType::RENDER_WIREFRAME;
        front_camera->position = Smith::Vector3(0.0f, 5.0f, 0.0f);
        front_camera->pitch = -90.0f; 
        front_camera->yaw = 0.0f;
        front_camera->UpdateVectors();

        Smith::App::Instance()->cameras.push_back(top_camera.get());
        Smith::App::Instance()->cameras.push_back(side_camera.get());
        Smith::App::Instance()->cameras.push_back(front_camera.get());

        main_camera = Smith::App::Instance()->main_camera.get();
        freecam_active = false;
        panning_camera_active = false;
        camera_2D_movement = false;
        current_tool = AnvilToolType::TOOL_SELECT;
        current_2D_moving_window = ViewWindow::VIEW_NONE;
        zoom_2d_view = false;
    }
    
    EditorGUI::~EditorGUI()
    {
    }

    void EditorGUI::UpdateCameraViewport(Smith::Camera3D* camera)
    {
        ImVec2 content_size = ImGui::GetContentRegionAvail();
        ImVec2 window_pos = ImGui::GetWindowPos();
        ImVec2 content_min = ImGui::GetWindowContentRegionMin();
        ImVec2 viewportPos(window_pos.x + content_min.x,
                        window_pos.y + content_min.y);

        Smith::Rect newRect = Smith::Rect(0, 0, content_size.x, content_size.y);

        if (newRect != camera->render_target.viewport_rect)
            camera->render_target.ResizeTarget(newRect);

    }
    
    void EditorGUI::SetupDockspace()
    {
        
        ImGuiID dockspace_id = ImGui::GetID("AnvilDockSpace");

        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

        ImGuiID top_toolbar = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.1f, nullptr, &dockspace_id);
        ImGui::DockBuilderDockWindow("Toolbar", top_toolbar);

        ImGuiID left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.5f, nullptr, &dockspace_id);
        ImGuiID right = dockspace_id;
        
        ImGuiID top_left = ImGui::DockBuilderSplitNode(left, ImGuiDir_Up, 0.5f, nullptr, &left);
        ImGuiID bottom_left = left;

        ImGuiID top_right = ImGui::DockBuilderSplitNode(right, ImGuiDir_Up, 0.5f, nullptr, &right);
        ImGuiID bottom_right = right;

        // Dock windows
        ImGui::DockBuilderDockWindow("3D View", top_left);
        ImGui::DockBuilderDockWindow("Top View", top_right);
        ImGui::DockBuilderDockWindow("Front View", bottom_left);
        ImGui::DockBuilderDockWindow("Side View", bottom_right);

        ImGui::DockBuilderFinish(dockspace_id);

        setup_dockspace = true;
    }

    void EditorGUI::Process()
    {

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoCloseButton;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("AnvilWindow", nullptr, window_flags);

        if (!setup_dockspace)
        {
            SetupDockspace();
        }

        ImGuiID dockspace_id = ImGui::GetID("AnvilDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New");
                ImGui::MenuItem("Open...");
                ImGui::MenuItem("Save");
                ImGui::MenuItem("Save As");
                ImGui::MenuItem("Exit");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Undo");
                ImGui::MenuItem("Redo");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();

        ImGui::Begin("3D View");
        UpdateCameraViewport(Smith::App::Instance()->main_camera.get());
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(Smith::App::Instance()->main_camera->render_target.color_texture)), ImGui::GetContentRegionAvail());
        cam_move_speed = Smith::Input::KeyPressed(Smith::Key::Shift) ? 10.0f : 4.0f;
        if (current_tool == AnvilToolType::TOOL_CAMERA)
        {   
            if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left) && !panning_camera_active)
            {
                freecam_active = true;
            }
            if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right) && !freecam_active)
            {
                panning_camera_active = true;
            }

            if (panning_camera_active)
            {
                Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_LOCKED);

                Smith::Vector2 mouse_delta = Smith::Input::GetMouseDelta();

                main_camera->UpdateVectors();

                float pan_speed = 0.5f * cam_move_speed;
                main_camera->position += main_camera->RIGHT * mouse_delta.x * pan_speed * Smith::App::Instance()->delta_time;
                main_camera->position += main_camera->UP * mouse_delta.y * pan_speed * Smith::App::Instance()->delta_time;

                if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
                {
                    panning_camera_active = false;
                    ImVec2 p = ImGui::GetWindowPos();
                    ImVec2 size = ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2);
                    SDL_WarpMouseInWindow(Smith::App::Instance()->window->raw_window, p.x + size.x, p.y + size.y);
                }

            }

            if (freecam_active)
            {
                Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_LOCKED);

                Smith::Vector2 mouse_delta = Smith::Input::GetMouseDelta();

                main_camera->yaw   += mouse_delta.x * cam_sensitivity * Smith::App::Instance()->delta_time;
                main_camera->pitch += mouse_delta.y * cam_sensitivity * Smith::App::Instance()->delta_time;
                main_camera->pitch = glm::clamp(main_camera->pitch, -89.0f, 89.0f); //Making it above these numbers causes weird rendering issues

                main_camera->UpdateVectors();

                Smith::Vector3 velocity(0.0f);
                if (Smith::Input::KeyPressed(Smith::Key::W))
                    velocity += main_camera->FRONT;
                if (Smith::Input::KeyPressed(Smith::Key::S))
                    velocity -= main_camera->FRONT;

                if (Smith::Input::KeyPressed(Smith::Key::D))
                    velocity += main_camera->RIGHT;
                if (Smith::Input::KeyPressed(Smith::Key::A))
                    velocity -= main_camera->RIGHT;

                if (Smith::Input::KeyPressed(Smith::Key::E))
                    velocity += main_camera->UP;
                if (Smith::Input::KeyPressed(Smith::Key::Q))
                    velocity -= main_camera->UP;

                if (glm::length(velocity) > 0.0f)
                    main_camera->position += glm::normalize(velocity) * cam_move_speed * Smith::App::Instance()->delta_time;  

                if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    freecam_active = false;
                    ImVec2 p = ImGui::GetWindowPos();
                    ImVec2 size = ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2);
                    SDL_WarpMouseInWindow(Smith::App::Instance()->window->raw_window, p.x + size.x, p.y + size.y);
                }

            }

            if (!panning_camera_active && !freecam_active)
            {
                Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_FREE);
            }
        }
        ImGui::End();

        ImGui::Begin("Top View");
        UpdateCameraViewport(top_camera.get());
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(top_camera->render_target.color_texture)), ImGui::GetContentRegionAvail());
        if (current_tool == AnvilToolType::TOOL_CAMERA)
        {
            if (current_2D_moving_window == ViewWindow::VIEW_NONE && ImGui::IsWindowHovered())
            {
                if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    current_2D_moving_window = ViewWindow::VIEW_TOP;
                }
                else if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
                {
                    current_2D_moving_window = ViewWindow::VIEW_TOP;
                    zoom_2d_view = true;
                }
                else
                {
                    Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_FREE);
                }
            }
            else if (current_2D_moving_window == ViewWindow::VIEW_TOP)
            {
                Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_LOCKED);
                if (zoom_2d_view)
                {
                    Smith::Vector2 mouse_delta = Smith::Input::GetMouseDelta();

                    top_camera->zoom += mouse_delta.y;

                    if (top_camera->zoom > 90)
                        top_camera->zoom = 90;
                    if (top_camera->zoom < 1)
                        top_camera->zoom = 1;

                    top_camera->UpdateVectors();
                    
                    if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
                    {
                        current_2D_moving_window = ViewWindow::VIEW_NONE;
                        zoom_2d_view = false;
                        ImVec2 p = ImGui::GetWindowPos();
                        ImVec2 size = ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2);
                        SDL_WarpMouseInWindow(Smith::App::Instance()->window->raw_window, p.x + size.x, p.y + size.y);
                    }
                }
                else
                {
                    Smith::Vector2 mouse_delta = Smith::Input::GetMouseDelta();

                    float pan_speed = 0.5f * cam_move_speed;
                    top_camera->position -= top_camera->RIGHT * mouse_delta.x * pan_speed * Smith::App::Instance()->delta_time;
                    top_camera->position -= top_camera->UP * mouse_delta.y * pan_speed * Smith::App::Instance()->delta_time;

                    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    {
                        current_2D_moving_window = ViewWindow::VIEW_NONE;
                        ImVec2 p = ImGui::GetWindowPos();
                        ImVec2 size = ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2);
                        SDL_WarpMouseInWindow(Smith::App::Instance()->window->raw_window, p.x + size.x, p.y + size.y);
                    }
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Side View");
        UpdateCameraViewport(side_camera.get());
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(side_camera->render_target.color_texture)), ImGui::GetContentRegionAvail());
        if (current_tool == AnvilToolType::TOOL_CAMERA)
        {
            if (current_2D_moving_window == ViewWindow::VIEW_NONE && ImGui::IsWindowHovered())
            {
                if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    current_2D_moving_window = ViewWindow::VIEW_SIDE;
                }
                else if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
                {
                    current_2D_moving_window = ViewWindow::VIEW_SIDE;
                    zoom_2d_view = true;
                }
                else
                {
                    Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_FREE);
                }
            }
            else if (current_2D_moving_window == ViewWindow::VIEW_SIDE)
            {
                Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_LOCKED);

                if (zoom_2d_view)
                {
                    Smith::Vector2 mouse_delta = Smith::Input::GetMouseDelta();

                    side_camera->zoom += mouse_delta.y;

                    if (side_camera->zoom > 90)
                        side_camera->zoom = 90;
                    if (side_camera->zoom < 1)
                        side_camera->zoom = 1;

                    side_camera->UpdateVectors();
                    
                    if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
                    {
                        current_2D_moving_window = ViewWindow::VIEW_NONE;
                        zoom_2d_view = false;
                        ImVec2 p = ImGui::GetWindowPos();
                        ImVec2 size = ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2);
                        SDL_WarpMouseInWindow(Smith::App::Instance()->window->raw_window, p.x + size.x, p.y + size.y);
                    }
                }
                else
                {
                    Smith::Vector2 mouse_delta = Smith::Input::GetMouseDelta();

                    float pan_speed = 0.5f * cam_move_speed;
                    side_camera->position -= side_camera->RIGHT * mouse_delta.x * pan_speed * Smith::App::Instance()->delta_time;
                    side_camera->position -= side_camera->UP * mouse_delta.y * pan_speed * Smith::App::Instance()->delta_time;

                    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    {
                        current_2D_moving_window = ViewWindow::VIEW_NONE;
                        ImVec2 p = ImGui::GetWindowPos();
                        ImVec2 size = ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2);
                        SDL_WarpMouseInWindow(Smith::App::Instance()->window->raw_window, p.x + size.x, p.y + size.y);
                    }
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Front View");
        UpdateCameraViewport(front_camera.get());
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(front_camera->render_target.color_texture)), ImGui::GetContentRegionAvail());
        if (current_tool == AnvilToolType::TOOL_CAMERA)
        {
            if (current_2D_moving_window == ViewWindow::VIEW_NONE && ImGui::IsWindowHovered())
            {
                if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    current_2D_moving_window = ViewWindow::VIEW_FRONT;
                }
                else if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
                {
                    current_2D_moving_window = ViewWindow::VIEW_FRONT;
                    zoom_2d_view = true;
                }
                else
                {
                    Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_FREE);
                }
            }
            else if (current_2D_moving_window == ViewWindow::VIEW_FRONT)
            {
                Smith::App::Instance()->SetCursorState(Smith::CursorState::STATE_CURSOR_LOCKED);

                if (zoom_2d_view)
                {
                    Smith::Vector2 mouse_delta = Smith::Input::GetMouseDelta();

                    front_camera->zoom += mouse_delta.y;

                    if (front_camera->zoom > 90)
                        front_camera->zoom = 90;
                    if (front_camera->zoom < 1)
                        front_camera->zoom = 1;

                    front_camera->UpdateVectors();
                    
                    if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
                    {
                        current_2D_moving_window = ViewWindow::VIEW_NONE;
                        zoom_2d_view = false;
                        ImVec2 p = ImGui::GetWindowPos();
                        ImVec2 size = ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2);
                        SDL_WarpMouseInWindow(Smith::App::Instance()->window->raw_window, p.x + size.x, p.y + size.y);
                    }
                }
                else
                {
                    Smith::Vector2 mouse_delta = Smith::Input::GetMouseDelta();

                    float pan_speed = 0.5f * cam_move_speed;
                    front_camera->position -= front_camera->RIGHT * mouse_delta.x * pan_speed * Smith::App::Instance()->delta_time;
                    front_camera->position -= front_camera->UP * mouse_delta.y * pan_speed * Smith::App::Instance()->delta_time;

                    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    {
                        current_2D_moving_window = ViewWindow::VIEW_NONE;
                        ImVec2 p = ImGui::GetWindowPos();
                        ImVec2 size = ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2);
                        SDL_WarpMouseInWindow(Smith::App::Instance()->window->raw_window, p.x + size.x, p.y + size.y);
                    }
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Toolbar");

        if (ImGui::Selectable("Selection", current_tool == AnvilToolType::TOOL_SELECT)) 
        {
            current_tool = AnvilToolType::TOOL_SELECT;
        }
        if (ImGui::BeginItemTooltip()) {
            ImGui::Text("Select brushes and actors");
            ImGui::EndTooltip();
        }

        if (ImGui::Selectable("Camera", current_tool == AnvilToolType::TOOL_CAMERA)) 
        {
            current_tool = AnvilToolType::TOOL_CAMERA;
        }
        if (ImGui::BeginItemTooltip()) {
            ImGui::Text("Use the camera in the viewports");
            ImGui::EndTooltip();
        }

        if (ImGui::Selectable("Actor", current_tool == AnvilToolType::TOOL_ACTOR)) 
        {
            current_tool = AnvilToolType::TOOL_ACTOR;
        }
        if (ImGui::BeginItemTooltip()) {
            ImGui::Text("Place an actor");
            ImGui::EndTooltip();
        }

        if (ImGui::Selectable("Brush", current_tool == AnvilToolType::TOOL_BRUSH)) 
        {
            current_tool = AnvilToolType::TOOL_BRUSH;
        }
        if (ImGui::BeginItemTooltip()) {
            ImGui::Text("Create a brush");
            ImGui::EndTooltip();
        }

        ImGui::End();

        if (ImGui::Button("test write"))
        {
            MapWriter::WriteMap();
        }
    }

    
} // namespace Anvil
