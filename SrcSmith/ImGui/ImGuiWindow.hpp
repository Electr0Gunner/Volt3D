#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_opengl3.h>
#include <vector>

namespace Smith
{
    class ImGuiWindow
    {
    public:
        ImGuiWindow() {};
        virtual ~ImGuiWindow() {};

        virtual void Process() = 0;

        bool is_open = true;
        int type_id = 0;
    };
    
} // namespace Smith
