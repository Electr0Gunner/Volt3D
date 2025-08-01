#pragma once

#include <imgui/imgui.h>
#include <vector>

namespace Volt
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
    
} // namespace Volt
