//
// KeyboardWindows.h
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "IKeyEventListener.h"

class KeyboardWindows
{
public:
    KeyboardWindows(IKeyEventListener* pListener);
    virtual ~KeyboardWindows();

    void SendKeyEvent(KeyEventType eventType, Key key);
    void SendKeyPress(Key key, bool shift, bool ctrl);

private:
    IKeyEventListener* m_pListener;

    HHOOK m_hKeyboardHook;

    bool m_bShiftPressed;
    bool m_bCtrlPressed;
    bool m_bAltPressed;

    bool ProcessKeyEvent(Key key, KeyEventType eventType);

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyEventType KeyEventTypeFromWParam(WPARAM wParam);

    static Key KeyFromKeyCode(int keyCode);
    static int KeyCodeFromKey(Key key);
};
