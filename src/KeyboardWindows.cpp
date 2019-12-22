//
// KeyboardWindows.cpp
//

#include "KeyboardWindows.h"

KeyboardWindows* g_pInstance = nullptr;

KeyboardWindows::KeyboardWindows()
{
    wxASSERT(g_pInstance == nullptr);

    g_pInstance = this;

    m_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardWindows::LowLevelKeyboardProc, NULL, 0);

    m_bShiftPressed = false;
    m_bCtrlPressed = false;
    m_bAltPressed = false;
}

KeyboardWindows::~KeyboardWindows()
{
    UnhookWindowsHookEx(m_hKeyboardHook);
}

void SendKeyboardEvent(bool keyUp, int vkCode)
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vkCode;
    input.ki.dwFlags = keyUp ? KEYEVENTF_KEYUP : 0;
    SendInput(1, &input, sizeof(INPUT));
}

void Press(int vkCode)
{
    SendKeyboardEvent(false, vkCode);
}

void Release(int vkCode)
{
    SendKeyboardEvent(true, vkCode);
}

void KeyboardWindows::SendKeyEvent(KeyEventType eventType, Key key)
{
    SendKeyboardEvent(eventType == KeyEventType::KeyUp, KeyCodeFromKey(key));
}

bool KeyboardWindows::OnKeyboardEvent(KeyEventType eventType, Key key)
{
    if (key == Key::Shift)
    {
        m_bShiftPressed = eventType == KeyEventType::KeyDown;
    }

    if (key == Key::Ctrl)
    {
        m_bCtrlPressed = eventType == KeyEventType::KeyDown;
    }

    if (key == Key::Alt)
    {
        m_bAltPressed = eventType == KeyEventType::KeyDown;
    }

    KeyTranslation kt = TranslateKey(key, m_bShiftPressed, m_bCtrlPressed, m_bAltPressed, Layout::DutchClassic);
    if (!kt.keystrokes.empty())
    {
        if (eventType == KeyEventType::KeyDown)
        {
            for (KeyStroke ks : kt.keystrokes)
            {
                if (m_bShiftPressed && !ks.shift)
                {
                    // Release Shift
                    Release(VK_SHIFT);
                }
                else if (!m_bShiftPressed && ks.shift)
                {
                    // Press Shift
                    Press(VK_SHIFT);
                }

                if (m_bCtrlPressed && !ks.ctrl)
                {
                    // Release Ctrl
                    Release(VK_CONTROL);
                }
                else if (!m_bCtrlPressed && ks.ctrl)
                {
                    // Press Ctrl
                    Press(VK_CONTROL);
                }

                wxLogDebug("Sending key event %d  Shift %d  Ctrl %d  ALt %d", ks.key, ks.shift, ks.ctrl, ks.alt);
                SendKeyEvent(KeyEventType::KeyDown, ks.key);
                SendKeyEvent(KeyEventType::KeyUp, ks.key);

                if (m_bShiftPressed && !ks.shift)
                {
                    // Re-press Shift
                    Press(VK_SHIFT);
                }
                else if (!m_bShiftPressed && ks.shift)
                {
                    // Release Shift
                    Release(VK_SHIFT);
                }

                if (m_bCtrlPressed && !ks.ctrl)
                {
                    // Re-press Ctrl
                    Press(VK_CONTROL);
                }
                else if (!m_bCtrlPressed && ks.ctrl)
                {
                    // Release Ctrl
                    Release(VK_CONTROL);
                }
            }
        }
        else
        {
            wxLogDebug("Ignoring event");
        }

        return true;
    }

    return false;
}

LRESULT CALLBACK KeyboardWindows::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KeyboardWindows* pThis = g_pInstance;

    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* pData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        wxLogDebug("LowLevelKeyboardProc(nCode = %d, wParam = %lld, lParam = %lld)  vkCode = 0x%x  scancode = %d  flags = 0x%x  time = %d  dwExtraInfo = 0x%llx",
            nCode, wParam, lParam, pData->vkCode, pData->scanCode, pData->flags, pData->time, pData->dwExtraInfo);

        KeyEventType eventType = wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN ? KeyEventType::KeyDown : KeyEventType::KeyUp;
        pThis->InvokeCallback(eventType, pData->vkCode, pData->scanCode, pData->flags, pData->time);

        // Ignore injected keystrokes
        if ((pData->flags & LLKHF_INJECTED) == 0)
        {
            Key key = KeyFromKeyCode(pData->vkCode);
            if (key != Key::Unknown)
            {
                if (pThis->OnKeyboardEvent(eventType, key))
                {
                    return 1;
                }
            }
        }
    }

    return CallNextHookEx(pThis->m_hKeyboardHook, nCode, wParam, lParam);
}

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
    { Key::OpenBracket, VK_OEM_4 },
    { Key::Backslash, VK_OEM_5 },
    { Key::CloseBracket, VK_OEM_6 },
    { Key::Apostrophe, VK_OEM_7 },
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
