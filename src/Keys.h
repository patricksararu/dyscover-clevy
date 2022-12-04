//
// Keys.h
//

#pragma once

#include <string>
#include <vector>

#include "Config.h"

enum class KeyEventType
{
    Unknown,
    KeyDown,
    KeyUp,
};

enum class Key
{
    Unknown,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Zero,
    Esc,
    Tab,
    CapsLock,
    Shift,
    Ctrl,
    WinCmd,
    Alt,
    Space,
    AltGr,
    Backspace,
    Enter,
    Apostrophe,
    Backslash,
    Semicolon,
    Comma,
    Dot,
    Minus,
    Slash,
    Equal,
    OpenBracket,
    CloseBracket,
    Ins,
    Del,
    Home,
    End,
    PageUp,
    PageDown,
    Up,
    Down,
    Left,
    Right,
    Backtick,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
};

enum KeyStrokeFlags {
    KSF_NoModifiers = 0,
    KSF_Shift = 1,
    KSF_Ctrl = 2,
    KSF_Alt = 3,
};

struct KeyStroke
{
    Key key;
    //bool shift;
    //bool ctrl;
    //bool alt;
    KeyStrokeFlags flags;
};

struct KeyTranslation
{
    std::vector<KeyStroke> keystrokes;
    std::string sound;

    bool speak_sentence;
};

std::string KeyToString(Key);
Key KeyFromString(std::string);

KeyTranslation TranslateKey(Key input, bool caps, bool shift, bool ctrl, bool alt, Layout layout);
