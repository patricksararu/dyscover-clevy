//
// KeyboardWindows.h
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Keyboard.h"

class KeyboardWindows : public Keyboard
{
public:
    KeyboardWindows();
    virtual ~KeyboardWindows();

private:
    HHOOK m_hKeyboardHook;

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static constexpr KeyMapping s_keyMappings[] = {
        { Key::Backspace, VK_BACK },
        { Key::Tab, VK_TAB },
        { Key::Enter, VK_RETURN },
        { Key::Shift, VK_SHIFT },
        { Key::Ctrl, VK_CONTROL },
        { Key::Esc, VK_ESCAPE },
        { Key::CapsLock, VK_CAPITAL },
        { Key::Space, VK_SPACE },
        { Key::PageUp, VK_PRIOR },
        { Key::PageDown, VK_NEXT },
        { Key::Home, VK_HOME },
        { Key::End, VK_END },
        { Key::Ins, VK_INSERT },
        { Key::Del, VK_DELETE },
        { Key::Up, VK_UP },
        { Key::Down, VK_DOWN },
        { Key::Left, VK_LEFT },
        { Key::Right, VK_RIGHT },
        { Key::WinCmd, VK_LWIN },
        { Key::Alt, VK_MENU },
        { Key::Zero, 0x30 },
        { Key::One, 0x31 },
        { Key::Two, 0x32 },
        { Key::Three, 0x33 },
        { Key::Four, 0x34 },
        { Key::Five, 0x35 },
        { Key::Six, 0x36 },
        { Key::Seven, 0x37 },
        { Key::Eight, 0x38 },
        { Key::Nine, 0x39 },
        { Key::A, 0x41 },
        { Key::B, 0x42 },
        { Key::C, 0x43 },
        { Key::D, 0x44 },
        { Key::E, 0x45 },
        { Key::F, 0x46 },
        { Key::G, 0x47 },
    };
};
