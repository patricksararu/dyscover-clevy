//
// Keys.h
//

#pragma once

#include <string>
#include <vector>

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
};

enum class Layout
{
    DutchClassic,
    DutchKWeC,
};

struct KeyTranslation
{
    std::vector<Key> keys;
    std::string sound;
};

std::string KeyToString(Key);
Key KeyFromString(std::string);

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
