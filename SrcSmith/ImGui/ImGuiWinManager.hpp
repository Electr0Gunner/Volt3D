#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "ImGuiWindow.hpp"

namespace Smith
{
    class ImGuiWinManager
    {
    public:
        ImGuiWinManager() {};
        ~ImGuiWinManager() {};

        void AddWindow(std::unique_ptr<ImGuiWindow> window)
        {
            ImGuiWindow* raw_ptr = window.get();
            auto it = std::find_if(windows.begin(), windows.end(),
            [raw_ptr](const std::unique_ptr<ImGuiWindow>& w)
            {
                return w->type_id == raw_ptr->type_id;
            }
            );
            if (it != windows.end())
                return;

            windows.push_back(std::move(window));
        }

        void Process()
        {
            for (auto& window : windows)
            {
                window->Process();
            }

            windows.erase(std::remove_if(windows.begin(), windows.end(), [](const std::unique_ptr<ImGuiWindow>& window)
            {
                return !window->is_open;
            }
            ), windows.end() );
        }
    private:
        std::vector<std::unique_ptr<ImGuiWindow>> windows;
    };
    
} // namespace Smith
