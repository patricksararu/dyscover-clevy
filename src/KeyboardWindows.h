//
// KeyboardWindows.h
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Keys.h"

class KeyboardWindows
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
    void SendKeyPress(Key key, bool shift, bool ctrl);

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyEventType KeyEventTypeFromWParam(WPARAM wParam);

    static Key KeyFromKeyCode(int keyCode);
    static int KeyCodeFromKey(Key key);
};
