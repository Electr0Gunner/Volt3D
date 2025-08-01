#include "Window.hpp"

using namespace Volt;
    
Window::Window(WindowProperties properties)
{
    SDL_WindowFlags translated_flags = 0;
    if (properties.flags & WindowFlags::FLAG_FULLSCREEN)
        translated_flags |= SDL_WINDOW_FULLSCREEN;
    if (properties.flags & WindowFlags::FLAG_RESIZABLE)
        translated_flags |= SDL_WINDOW_RESIZABLE;

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    raw_window = SDL_CreateWindow(properties.title.c_str(), properties.width, properties.height, translated_flags  | SDL_WINDOW_OPENGL);
}
    
Window::~Window()
{
    SDL_DestroyWindow(raw_window);

}
    