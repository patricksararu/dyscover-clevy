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
    KeyboardWindows(IKeyEventListener* pListener);
    virtual ~KeyboardWindows();

    virtual void SendKeyEvent(KeyEventType eventType, Key key) override;

    virtual std::string TranslateKeyStroke(Key key, bool shift, bool ctrl) override;

private:
    HHOOK m_hKeyboardHook;

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyEventType KeyEventTypeFromWParam(WPARAM wParam);

    static Key KeyFromKeyCode(int keyCode);
    static int KeyCodeFromKey(Key key);
};
