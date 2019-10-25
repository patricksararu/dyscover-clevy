//
// Keyboard.h
//

#pragma once

enum class Key { Esc, Num1, Num2, OpenBracket, Backspace, Tab, Ctrl, Shift, Alt, WinCmd, AltGr, Ins, Del, Home, End, Up, Down, Left, Right };

enum class KeyEventType { KeyDown, KeyUp };

typedef void (*PFNKBDHOOKCB)(KeyEventType eventType, int keyCode, int scanCode, int flags, int time, void* pUser);

class Keyboard
{
public:
    static Keyboard* create(PFNKBDHOOKCB, void* pUser);

    //virtual void Send(KeyEventType, Key) = 0;

protected:
    //Keyboard();
    void InvokeCallback(KeyEventType keyEventType, int keyCode, int scanCode, int flags, int time);

private:
    PFNKBDHOOKCB m_pfnCallback;
    void* m_pUserData;
};
