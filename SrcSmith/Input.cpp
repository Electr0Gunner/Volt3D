#include "Input.hpp"

using namespace Smith;

std::bitset<static_cast<size_t>(KEY_COUNT)> Smith::Input::keys_pressed;
std::bitset<static_cast<size_t>(KEY_COUNT)> Smith::Input::keys_held;
std::bitset<static_cast<size_t>(KEY_COUNT)> Smith::Input::keys_released;
Vector2 Smith::Input::mouse_position;
Vector2 Smith::Input::mouse_delta;

void Input::Init()
{
    mouse_position = {0, 0};
    mouse_delta = {0, 0};
    keys_released.reset();
    keys_pressed.reset();
}

void Input::Update()
{
    mouse_delta = {0, 0};
    keys_pressed.reset();
    keys_released.reset();
}

void Input::OnEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        Key key = ToSmithKey(event.key.key);
        if (!keys_held[key])
            keys_pressed[key] = true;

        keys_held[key] = true;
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        Key key = ToSmithKey(event.key.key);
        keys_released[key] = true;
        keys_held[key] = false;
    }
    else if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        mouse_delta =  Vector2(event.motion.xrel, event.motion.yrel);
        mouse_position = Vector2(event.motion.x, event.motion.y);
    }
}
bool Input::KeyJustPressed(Key key)
{
    return keys_pressed[key];
}

bool Input::KeyPressed(Key key)
{
    return keys_held[key];
}

bool Input::KeyReleased(Key key)
{
    return keys_released[key];
}

Vector2 Input::GetMousePosition()
{
    return mouse_position;
}

Vector2 Input::GetMouseDelta()
{
    return mouse_delta;
}