#pragma once

#include <string>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

namespace Smith
{
    enum WindowFlags
    {
        FLAG_FULLSCREEN,
        FLAG_RESIZABLE,
    };

    struct WindowProperties
    {
        std::string title;
        int width;
        int height;
        WindowFlags flags;
    };

    class Window
    {
    public:
        Window(WindowProperties properties);
        ~Window();

        SDL_Window* raw_window;
    };
} // namespace Smith
