#include "App.hpp"
#include "Random.hpp"
#include "Input.hpp"
#include "ResourceManager.hpp"
#include "ImGui/DebugInfoWindow.hpp"

#include "graphics/Renderer3D.hpp"
#include <SDL3/SDL.h>
#include <algorithm>

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

using namespace Smith;

App* Smith::App::instance = nullptr;
    
App::App(/* args */)
{
    SDL_Init(SDL_INIT_VIDEO);

    instance = this;

    game_width = window_width = 1600;
    game_height = window_height = 900;

    shutdown = false;
    error_occured = false;
    window = nullptr;
    title_label = "SrcSmith";

    frame_count = 0;
    framerate = 0;
    error_occured = false;
}
    
App::~App()
{
    cameras.clear();
    SetCursorState();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_Quit();
}

AppInitCode App::Init()
{
    Random::Seed(SDL_GetTicks());

    int win_flags;
    win_flags |= FLAG_RESIZABLE;
    WindowProperties props{title_label, window_width, window_height, (WindowFlags)win_flags};
    window = std::make_unique<Window>(props);

    renderer = std::make_unique<Renderer3D>();
    renderer->Init();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window->raw_window, renderer->GetContext());
    ImGui_ImplOpenGL3_Init("#version 330 core");

    imgui_manager = std::make_unique<ImGuiWinManager>();
    main_camera = std::make_shared<Camera3D>();
    main_camera->render_target = renderer->CreateRenderTarget();
    cameras.push_back(main_camera.get());
    
    ResourceManager::Init();

    return AppInitCode::CODE_APP_SUCCESS;
}

void App::Start()
{
    DoGameLoop();
}

void App::DoGameLoop()
{
    previous_tick = SDL_GetTicks();

    while (!shutdown && !error_occured)
    {
        current_tick  = SDL_GetTicks();
        delta_time = (current_tick - previous_tick) / 1000.0f; // seconds

        Input::Update();
        ParseEvents();
        renderer->BeginFrame();
        
        imgui_manager->Process();

        renderer->Render();

        renderer->EndFrame();

        CalculateFramerate();

        
        previous_tick = current_tick; // update the previous tick after all processing is done
    }
}

void App::ParseEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        Input::OnEvent(event);
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            shutdown = true;
            break;
        case SDL_EVENT_KEY_UP:
            if (event.key.key == SDLK_F1)
                SetCursorState();
            else if (event.key.key == SDLK_F2)
                SetCursorState(CursorState::STATE_CURSOR_LOCKED);
            else if (event.key.key == SDLK_F3)
                imgui_manager->AddWindow(std::make_unique<DebugInfoWindow>());
            break;
        
        default:
            break;
        }
    }
    
}

void App::CalculateFramerate()
{
    frame_count++;
    if (!(frame_count % 20))
    {
        framerate = (frame_count * 1000.f) / (current_tick - previous_tick);
        frame_count = 0;
    }
}

void App::SetCursorState(CursorState state)
{
    switch (state)
    {
    case CursorState::STATE_CURSOR_FREE:
        SDL_SetWindowRelativeMouseMode(window->raw_window, false);
        break;
    
    case CursorState::STATE_CURSOR_LOCKED:
        SDL_SetWindowRelativeMouseMode(window->raw_window, true);
        break;
    }
}