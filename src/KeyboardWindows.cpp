//
// KeyboardWindows.cpp
//

#include <stdio.h>

#include "KeyboardWindows.h"

//KeyboardWindows::s_pInstance;
KeyboardWindows* g_pInstance = nullptr;

KeyboardWindows::KeyboardWindows()
{
    //DEBUG_ASSERT(g_pInstance == nullptr);

    g_pInstance = this;

    m_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardWindows::LowLevelKeyboardProc, NULL, 0);
}

KeyboardWindows::~KeyboardWindows()
{
    UnhookWindowsHookEx(m_hKeyboardHook);
}

void SendKeyEvent()
{

}

LRESULT CALLBACK KeyboardWindows::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KeyboardWindows* pThis = g_pInstance;

    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* pData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        printf("LowLevelKeyboardProc(nCode = %d, wParam = %lld, lParam = %lld)  vkCode = %d  scancode = %d  flags = %x  time = %d  dwExtraInfo = %llx\n",
            nCode, wParam, lParam, pData->vkCode, pData->scanCode, pData->flags, pData->time, pData->dwExtraInfo);

        KeyEventType eventType = wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN ? KeyEventType::KeyDown : KeyEventType::KeyUp;
        pThis->InvokeCallback(eventType, pData->vkCode, pData->scanCode, pData->flags, pData->time);

        // Ignore injected keystrokes
        if ((pData->flags & LLKHF_INJECTED) == 0)
        {
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
        }
    }

    //LRESULT lResult = 
    //printf("> CallNextHookEx() returned %lld\n", lResult);
    return CallNextHookEx(pThis->m_hKeyboardHook, nCode, wParam, lParam);
}
