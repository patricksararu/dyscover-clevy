//
// KeyboardWindows.cpp
//

#include "KeyboardWindows.h"

//KeyboardWindows::s_pInstance;
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
    if (!kt.keys.empty())
    {
        if (eventType == KeyEventType::KeyDown)
        {
            for (KeyStroke ks : kt.keys)
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

#if 0
            if (pData->vkCode == 49)
            {
                INPUT input;
                ZeroMemory(&input, sizeof(INPUT));
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = 65;
                input.ki.dwFlags = wParam == WM_KEYUP ? KEYEVENTF_KEYUP : 0;
                SendInput(1, &input, sizeof(INPUT));
                SendInput(1, &input, sizeof(INPUT));

                return 1;
                //supress = true;
            }
#endif
        }
    }

    //LRESULT lResult = 
    //printf("> CallNextHookEx() returned %lld\n", lResult);
    return CallNextHookEx(pThis->m_hKeyboardHook, nCode, wParam, lParam);
}

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
