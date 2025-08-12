#pragma once

#include "App.hpp"
#include "Common.hpp"
#include "Random.hpp"
#include "ImGuiWindow.hpp"

namespace Smith
{
    class DebugInfoWindow : public ImGuiWindow
    {
    public:
        DebugInfoWindow();
        ~DebugInfoWindow();

        virtual void Process();
        int random_num = 0;
        float random_float = 0.0f;
        bool random_bool = false;
    };
    
    DebugInfoWindow::DebugInfoWindow() : ImGuiWindow()
    {
        type_id = -1; //type id meant to avoid adding the same window type multiple times
    }
    
    DebugInfoWindow::~DebugInfoWindow()
    {
    }

    void DebugInfoWindow::Process()
    {
        ImGui::Begin("SrcSmith-debug", &is_open);

        ImGui::Text("SrcSmith version:%s, %s build", SRCSMITH_VERSION, SRCSMITH_BUILD_TYPE);
        ImGui::Text("FPS: %.2f", App::Instance()->framerate);
        Camera3D* camera = App::Instance()->cameras.front();
        ImGui::Text("Camera Position:\nX:%.6f\nY:%.6f\nZ:%.6f", camera->position.x, camera->position.y, camera->position.z);
        ImGui::Text("%s", App::Instance()->renderer->GetErrorStr().c_str());
        if (ImGui::Button("Random number"))
        {
            random_num = Random::Int();
        }
        ImGui::Text("%i", random_num);
        if (ImGui::Button("Random float"))
        {
            random_float = Random::Float();
        }
        ImGui::Text("%f", random_float);
        if (ImGui::Button("Random bool"))
        {
            random_bool = Random::Bool();
        }
        ImGui::Text("%s", random_bool ? "true" : "false");

        ImGui::End();
    }
        
} // namespace Smith
