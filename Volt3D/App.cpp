#include "App.hpp"
#include "Random.hpp"
#include "Input.hpp"
#include "ImGui/DebugInfoWindow.hpp"

#include "graphics/Renderer3D.hpp"
#include <SDL3/SDL.h>
#include <algorithm>

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

using namespace Volt;

App* Volt::App::instance = nullptr;
    
App::App(/* args */)
{
    SDL_Init(SDL_INIT_VIDEO);

    instance = this;

    shutdown = false;
    error_occured = false;
    window = nullptr;
    title_label = "Volt3D";

    frame_count = 0;
    framerate = 0;
    error_occured = false;
}
    
App::~App()
{
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
    WindowProperties props{title_label, 1600, 900, (WindowFlags)win_flags};
    window = std::make_unique<Window>(props);

    renderer = std::make_unique<Renderer3D>(1);
    renderer->Init();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window->raw_window, renderer->GetContext());
    ImGui_ImplOpenGL3_Init("#version 330 core");

    imgui_manager = std::make_unique<ImGuiWinManager>();
    camera = std::make_unique<Camera3D>();

    return AppInitCode::CODE_APP_SUCCESS;
}

void App::Start()
{
    DoGameLoop();
}

bool show_demo_window = true;

void App::DoGameLoop()
{
    previous_tick = SDL_GetTicks();

    while (!shutdown && !error_occured)
    {
        current_tick  = SDL_GetTicks();
        delta_time = (current_tick - previous_tick) / 1000.0f; // seconds

        float vel = camera->speed * delta_time;

        if (Input::KeyPressed(Key::W)) camera->position += camera->FRONT * vel;
        if (Input::KeyPressed(Key::S)) camera->position -= camera->FRONT * vel;
        if (Input::KeyPressed(Key::A)) camera->position -= camera->RIGHT * vel;
        if (Input::KeyPressed(Key::D)) camera->position += camera->RIGHT * vel;
        if (Input::KeyPressed(Key::Space)) camera->position += camera->UP * vel;
        if (Input::KeyPressed(Key::Shift)) camera->position -= camera->UP * vel;

        Vector2 mouse_delta = Input::GetMouseDelta();

        if (mouse_delta.x != 0.0f || mouse_delta.y != 0.0f)
        {
            camera->yaw += mouse_delta.x * camera->sensitivity;
            camera->pitch -= mouse_delta.y * camera->sensitivity;
            camera->pitch = std::clamp(camera->pitch, -89.0f, 89.0f);
            camera->UpdateVectors();
        }

        Input::Update();
        ParseEvents();
        renderer->BeginFrame();
        
        imgui_manager->Process();
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

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
        SDL_WarpMouseInWindow(window->raw_window, 1600 / 2, 900 / 2);
        break;
    
    case CursorState::STATE_CURSOR_LOCKED:
        SDL_SetWindowRelativeMouseMode(window->raw_window, true);
        SDL_SetWindowMouseRect(window->raw_window, nullptr);
        break;
    }
}