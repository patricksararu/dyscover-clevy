//
// Keys.h
//

#pragma once

enum class Key
{
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
    Minus,
    Slash,
    Equal,
    Ins,
    Del,
    Home,
    End,
    PageUp,
    PageDown,
    OpenBracket,
    CloseBracket,
    Up,
    Down,
    Left,
    Right,
};

enum class Layout
{
    DutchClassic,
    DutchKWeC,
};

struct KeyTranslation
{
    Key* keys;
    char* sound;
};

/*
KeyTranslation(Key* keys, char* sound) : keys(keys), sound(sound) {}
KeyTranslation(const KeyTranslation& other) : keys(other.keys), sound(other.sound) {}
KeyTranslation(KeyTranslation&& other) : keys(other.keys), sound(other.sound) {}

constexpr KeyTranslation& operator=(const KeyTranslation& other)
{
    keys = other.keys;
    sound = other.sound;
}
*/

KeyTranslation TranslateKey(Key input, bool shift, bool ctrl, bool alt, Layout layout);
