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

    bool m_bShiftPressed;
    bool m_bCtrlPressed;
    bool m_bAltPressed;

    void SendKeyEvent(KeyEventType eventType, Key key);

    bool OnKeyboardEvent(KeyEventType eventType, Key key);

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static Key KeyFromKeyCode(int keyCode);
    static int KeyCodeFromKey(Key key);
};
