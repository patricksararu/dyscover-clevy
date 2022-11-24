//
// KeyboardWindows.cpp
//

#include <cassert>

#include "KeyboardWindows.h"

KeyboardWindows* g_pInstance = nullptr;

KeyboardWindows::KeyboardWindows(IKeyEventListener* pListener)
    : Keyboard(pListener)
{
    assert(g_pInstance == nullptr);

    g_pInstance = this;

    m_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardWindows::LowLevelKeyboardProc, NULL, 0);
}

KeyboardWindows::~KeyboardWindows()
{
    UnhookWindowsHookEx(m_hKeyboardHook);
}

bool KeyboardWindows::IsCapsLockActive()
{
    return GetKeyState(VK_CAPITAL) & 1;
}

void KeyboardWindows::SendKeyEvent(KeyEventType eventType, Key key)
{
    int keyCode = KeyCodeFromKey(key);
    if (keyCode != -1)
    {
        INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = static_cast<WORD>(keyCode);
        input.ki.dwFlags = eventType == KeyEventType::KeyUp ? KEYEVENTF_KEYUP : 0;
        SendInput(1, &input, sizeof(INPUT));
    }
}

LRESULT CALLBACK KeyboardWindows::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KeyboardWindows* pThis = g_pInstance;

    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* pData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        // Ignore generated keystrokes in order to prevent (infinite) recursion
        if ((pData->flags & LLKHF_INJECTED) == 0)
        {
            KeyEventType eventType = KeyEventTypeFromWParam(wParam);
            Key key = KeyFromKeyCode(pData->vkCode);

            if (pThis->ProcessKeyEvent(eventType, key))
            {
                // Supress this event
                return 1;
            }
        }
    }

    return CallNextHookEx(pThis->m_hKeyboardHook, nCode, wParam, lParam);
}

std::string KeyboardWindows::TranslateKeyStroke(Key key, bool shift, bool ctrl)
{
    int vkCode = KeyCodeFromKey(key);

    // Check if keystroke is a dead key/diacritic.
    // This is needed because ToAscii() modifies the keyboard state and effectively kills diacritics.
    // https://stackoverflow.com/questions/1964614/toascii-tounicode-in-a-keyboard-hook-destroys-dead-keys
    UINT mapped = MapVirtualKey(vkCode, MAPVK_VK_TO_CHAR);
    if (mapped >> (sizeof(UINT) * 8 - 1) & 1)  return std::string();

    BYTE keyboardState[256];
    if (GetKeyboardState(keyboardState) == 0)  return std::string();

    keyboardState[VK_SHIFT] = shift ? 0xFF : 0x00;
    keyboardState[VK_CONTROL] = ctrl ? 0xFF : 0x00;

    WORD charBuffer[10];
    ZeroMemory(charBuffer, sizeof(charBuffer));

    // TODO: Use ToUnicode().
    int result = ToAscii(vkCode, 0, keyboardState, charBuffer, 0);
    if (result < 0)  return std::string();

    return std::string(reinterpret_cast<char*>(&charBuffer[0]));
}

struct KeyMapping
{
    Key key;
    int code;
};

static constexpr KeyMapping s_keyMappings[] = {
    { Key::Backspace, VK_BACK },
    { Key::Tab, VK_TAB },
    { Key::Enter, VK_RETURN },
    { Key::Shift, VK_LSHIFT },
    { Key::Ctrl, VK_LCONTROL },
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
    { Key::Alt, VK_LMENU },
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
    { Key::H, 0x48 },
    { Key::I, 0x49 },
    { Key::J, 0x4A },
    { Key::K, 0x4B },
    { Key::L, 0x4C },
    { Key::M, 0x4D },
    { Key::N, 0x4E },
    { Key::O, 0x4F },
    { Key::P, 0x50 },
    { Key::Q, 0x51 },
    { Key::R, 0x52 },
    { Key::S, 0x53 },
    { Key::T, 0x54 },
    { Key::U, 0x55 },
    { Key::V, 0x56 },
    { Key::W, 0x57 },
    { Key::X, 0x58 },
    { Key::Y, 0x59 },
    { Key::Z, 0x5A },
    { Key::AltGr, VK_RMENU },
    { Key::Equal, VK_OEM_PLUS },
    { Key::Comma, VK_OEM_COMMA },
    { Key::Minus, VK_OEM_MINUS },
    { Key::Dot, VK_OEM_PERIOD },
    { Key::Semicolon, VK_OEM_1 },
    { Key::Slash, VK_OEM_2 },
    { Key::Backtick, VK_OEM_3 },
    { Key::OpenBracket, VK_OEM_4 },
    { Key::Backslash, VK_OEM_5 },
    { Key::CloseBracket, VK_OEM_6 },
    { Key::Apostrophe, VK_OEM_7 },
    { Key::F1, VK_F1 },
    { Key::F2, VK_F2 },
    { Key::F3, VK_F3 },
    { Key::F4, VK_F4 },
    { Key::F5, VK_F5 },
    { Key::F6, VK_F6 },
    { Key::F7, VK_F7 },
    { Key::F8, VK_F8 },
    { Key::F9, VK_F9 },
    { Key::F10, VK_F10 },
    { Key::F11, VK_F11 },
    { Key::F12, VK_F12 },
};

Key KeyboardWindows::KeyFromKeyCode(int keyCode)
{
    for (KeyMapping mapping : s_keyMappings)
    {
        if (mapping.code == keyCode)
        {
            return mapping.key;
        }
    }
    return Key::Unknown;
}

int KeyboardWindows::KeyCodeFromKey(Key key)
{
    for (KeyMapping mapping : s_keyMappings)
    {
        if (mapping.key == key)
        {
            return mapping.code;
        }
    }
    return -1;
}

KeyEventType KeyboardWindows::KeyEventTypeFromWParam(WPARAM wParam)
{
    switch (wParam)
    {
    case WM_KEYUP:
    case WM_SYSKEYUP:
        return KeyEventType::KeyUp;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        return KeyEventType::KeyDown;
    default:
        return KeyEventType::Unknown;
    }
}
