#pragma once

#include "Window.hpp"
#include "graphics/Renderer3D.hpp"
#include "graphics/Camera3D.hpp"
#include "ImGui/ImGuiWinManager.hpp"

#include <memory>

namespace Smith
{
    enum AppInitCode
    {
        CODE_APP_SUCCESS,
        CODE_APP_FAIL,
    };

    enum CursorState
    {
        STATE_CURSOR_FREE,
        STATE_CURSOR_LOCKED,
    };

    class App
    {

    private:
        void DoGameLoop();
        void ParseEvents();
        void CalculateFramerate();

        int frame_count;
        uint64_t current_tick;
        uint64_t previous_tick;
    public:
        App(/* args */);
        ~App();

        AppInitCode Init();
        void Start();
        void SetCursorState(CursorState state = CursorState::STATE_CURSOR_FREE);


        static App* Instance()
        {
            return instance;
        }

        int game_width;
        int game_height;
        int window_width;
        int window_height;
        std::string title_label;
        bool shutdown;
        bool error_occured;
        std::unique_ptr<Window> window;
        std::unique_ptr<Renderer3D> renderer;
        std::shared_ptr<Camera3D> main_camera;
        std::vector<Camera3D*> cameras;
        std::unique_ptr<ImGuiWinManager> imgui_manager;
        float delta_time;
        float framerate;

        static App* instance;
    };

    
} // namespace Smith