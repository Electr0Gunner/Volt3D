#pragma once

#include <Common.hpp>

#include <vector>
#include <bitset>

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_events.h>

namespace Smith
{

    enum MouseButton
    {
        MouseLeft,
        MouseRight,
        MouseMiddle,
        MouseX1,
        MouseX2,
    };

    enum Key
    {
        UNKNOWN,
        A, B, C, D, E, F, G,
        H, I, J, K, L, M, N,
        O, P, Q, R, S, T, U,
        V, W, X, Y, Z,
        Num0, Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8, Num9,
        F1, F2, F3, F4, F5, F6,
        F7, F8, F9, F10, F11, F12,
        Keypad0, Keypad1, Keypad2, Keypad3, Keypad4,
        Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
        KeypadPlus, KeypadMinus, KeypadMultiply, KeypadDivide,
        KeypadPeriod, KeypadEqual,
        Up, Down, Left, Right,
        Plus, // +
        Minus, // -
        Equals, // =
        Asterisk, // *
        Slash, //  /
        Backslash, // "\"
        Colon, // :
        Semicolon, // ;
        Apostrophe, // "
        Comma, // ,
        Period, // .
        Space,
        Escape,
        Tab,
        Shift, //Left or right
        Control, //Left or right
        Alt, //Left and right
        Command, //Windows/Command key (Left and right)
        Enter, //Left and right
        CapsLock,
        Backspace,
        Delete,
        Insert,
        Home,
        End,
        PageUp,
        PageDown,
        PrintScreen,
        ScrollLock,
        Pause,
        NumLock,
        KEY_COUNT,
    };

    // Oh god this is horrible.
    
    static const std::vector<std::pair<SDL_Keycode, Key>> key_translation = {
    { SDLK_A, Key::A },
    { SDLK_B, Key::B },
    { SDLK_C, Key::C },
    { SDLK_D, Key::D },
    { SDLK_E, Key::E },
    { SDLK_F, Key::F },
    { SDLK_G, Key::G },
    { SDLK_H, Key::H },
    { SDLK_I, Key::I },
    { SDLK_J, Key::J },
    { SDLK_K, Key::K },
    { SDLK_L, Key::L },
    { SDLK_M, Key::M },
    { SDLK_N, Key::N },
    { SDLK_O, Key::O },
    { SDLK_P, Key::P },
    { SDLK_Q, Key::Q },
    { SDLK_R, Key::R },
    { SDLK_S, Key::S },
    { SDLK_T, Key::T },
    { SDLK_U, Key::U },
    { SDLK_V, Key::V },
    { SDLK_W, Key::W },
    { SDLK_X, Key::X },
    { SDLK_Y, Key::Y },
    { SDLK_Z, Key::Z },
    { SDLK_0, Key::Num0 },
    { SDLK_1, Key::Num1 },
    { SDLK_2, Key::Num2 },
    { SDLK_3, Key::Num3 },
    { SDLK_4, Key::Num4 },
    { SDLK_5, Key::Num5 },
    { SDLK_6, Key::Num6 },
    { SDLK_7, Key::Num7 },
    { SDLK_8, Key::Num8 },
    { SDLK_9, Key::Num9 },
    { SDLK_F1, Key::F1 },
    { SDLK_F2, Key::F2 },
    { SDLK_F3, Key::F3 },
    { SDLK_F4, Key::F4 },
    { SDLK_F5, Key::F5 },
    { SDLK_F6, Key::F6 },
    { SDLK_F7, Key::F7 },
    { SDLK_F8, Key::F8 },
    { SDLK_F9, Key::F9 },
    { SDLK_F10, Key::F10 },
    { SDLK_F11, Key::F11 },
    { SDLK_F12, Key::F12 },
    { SDLK_KP_0, Key::Keypad0 },
    { SDLK_KP_1, Key::Keypad1 },
    { SDLK_KP_2, Key::Keypad2 },
    { SDLK_KP_3, Key::Keypad3 },
    { SDLK_KP_4, Key::Keypad4 },
    { SDLK_KP_5, Key::Keypad5 },
    { SDLK_KP_6, Key::Keypad6 },
    { SDLK_KP_7, Key::Keypad7 },
    { SDLK_KP_8, Key::Keypad8 },
    { SDLK_KP_9, Key::Keypad9 },
    { SDLK_KP_PLUS, Key::KeypadPlus},
    { SDLK_KP_MINUS, Key::KeypadMinus},
    { SDLK_KP_MULTIPLY, Key::KeypadMultiply},
    { SDLK_KP_DIVIDE, Key::KeypadDivide},
    { SDLK_KP_PERIOD, Key::KeypadPeriod},
    { SDLK_KP_EQUALS, Key::KeypadEqual},
    { SDLK_UP, Key::Up},
    { SDLK_DOWN, Key::Down},
    { SDLK_LEFT, Key::Left},
    { SDLK_RIGHT, Key::Right},
    { SDLK_PLUS, Key::Plus},
    { SDLK_MINUS, Key::Minus},
    { SDLK_EQUALS, Key::Equals},
    { SDLK_ASTERISK, Key::Asterisk},
    { SDLK_SLASH, Key::Slash},
    { SDLK_BACKSLASH, Key::Backslash},
    { SDLK_COLON, Key::Colon},
    { SDLK_SEMICOLON, Key::Semicolon},
    { SDLK_PERIOD, Key::Period},
    { SDLK_SPACE, Key::Space},
    { SDLK_ESCAPE, Key::Escape},
    { SDLK_TAB, Key::Tab},
    { SDLK_LSHIFT, Key::Shift},
    { SDLK_RSHIFT, Key::Shift},
    { SDLK_LCTRL, Key::Control},
    { SDLK_RCTRL, Key::Control},
    { SDLK_LALT, Key::Alt},
    { SDLK_RALT, Key::Alt},
    { SDLK_LGUI, Key::Command},
    { SDLK_RGUI, Key::Command},
    { SDLK_RETURN, Key::Enter},
    { SDLK_RETURN2, Key::Enter},
    { SDLK_CAPSLOCK, Key::CapsLock},
    { SDLK_BACKSPACE, Key::Backspace},
    { SDLK_DELETE, Key::Delete},
    { SDLK_INSERT, Key::Insert},
    { SDLK_HOME, Key::Home},
    { SDLK_END, Key::End},
    { SDLK_PAGEUP, Key::PageUp},
    { SDLK_PAGEDOWN, Key::PageDown},
    { SDLK_PRINTSCREEN, Key::PrintScreen},
    { SDLK_SCROLLLOCK, Key::ScrollLock},
    { SDLK_PAUSE, Key::Pause},
    { SDLK_NUMLOCKCLEAR, Key::NumLock},
    };

    inline Key ToSmithKey(SDL_Keycode key)
    {
        for (const auto& pair : key_translation)
        {
            if (pair.first == key)
                return pair.second;
        }
        return Key::UNKNOWN;
    }

    inline SDL_Keycode ToSDLKeyCode(Key key)
    {
        for (const auto& pair : key_translation)
        {
            if (pair.second == key)
                return pair.first;
        }
        return SDLK_UNKNOWN;
    }

    inline const char* GetKeyName(Key key)
    {
        SDL_Keycode keycode = ToSDLKeyCode(key);
        return SDL_GetKeyName(keycode);
    }


    class Input
    {
    private:
        static std::bitset<static_cast<size_t>(KEY_COUNT)> keys_pressed;
        static std::bitset<static_cast<size_t>(KEY_COUNT)> keys_held;
        static std::bitset<static_cast<size_t>(KEY_COUNT)> keys_released;
        static Vector2 mouse_position;
        static Vector2 mouse_delta;

    public:
        static void Init();
        static void Update();
        static void OnEvent(const SDL_Event& event);

        static bool KeyJustPressed(Key key);
        static bool KeyPressed(Key key);
        static bool KeyReleased(Key key);
        static Vector2 GetMousePosition();
        static Vector2 GetMouseDelta();
    };

} // namespace Smith
