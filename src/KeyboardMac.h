//
// KeyboardMac.h
//

#pragma once

#include "Keyboard.h"

class KeyboardMac : public Keyboard
{
public:
    KeyboardMac();
    virtual ~KeyboardMac();

private:
    static constexpr KeyMapping s_keyMappings[] = {
      { Key::Up, 126 },
      { Key::Down, 125 },
      { Key::Left, 123 },
      { Key::Right, 124 },
      { Key::Backspace, 117 },
      { Key::Enter, 76 },
      { Key::Home, 115 },
      { Key::End, 119 },
      { Key::PageUp, 116 },
      { Key::PageDown, 121 },
      { Key::Enter, 36 },
      { Key::Del, 51 },
      { Key::Tab, 48 },
      { Key::Space, 49 },
      { Key::Shift, 56 },
      { Key::Ctrl, 59 },
      { Key::Alt, 58 },
      { Key::Esc, 53 },
      { Key::CapsLock, 57 },
      { Key::WinCmd, 55 },
      { Key::Q, 12 },
      { Key::W, 13 },
      { Key::E, 14 },
      { Key::R, 15 },
      { Key::T, 17 },
      { Key::Y, 16 },
      { Key::U, 32 },
      { Key::I, 34 },
      { Key::O, 31 },
      { Key::P, 35 },
      { Key::A, 0 },
      { Key::S, 1 },
      { Key::D, 2 },
      { Key::F, 3 },
      { Key::G, 5 },
      { Key::H, 4 },
      { Key::J, 38 },
      { Key::K, 40 },
      { Key::L, 37 },
      { Key::Z, 6 },
      { Key::X, 7 },
      { Key::C, 8 },
      { Key::V, 9 },
      { Key::B, 11 },
      { Key::N, 45 },
      { Key::M, 46 },
      { Key::Zero, 29 },
      { Key::One, 18 },
      { Key::Two, 19 },
      { Key::Three, 20 },
      { Key::Four, 21 },
      { Key::Five, 23 },
      { Key::Six, 22 },
      { Key::Seven, 26 },
      { Key::Eight, 28 },
      { Key::Nine, 25 },
    };
};
