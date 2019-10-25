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

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
};
