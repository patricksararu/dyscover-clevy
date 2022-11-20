//
// Keyboard.h
//

#pragma once

#include "Keys.h"

class IKeyEventListener
{
public:
    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool capsLock, bool shift, bool ctrl, bool alt) = 0;
};

class Keyboard
{
public:
    static Keyboard* Create(IKeyEventListener*);

    Keyboard(IKeyEventListener*);
    virtual ~Keyboard();

    virtual void SendKeyEvent(KeyEventType eventType, Key key) = 0;

    void SendKeyStroke(Key key, bool shift, bool ctrl, bool alt);

    virtual std::string TranslateKeyStroke(Key key, bool shift, bool ctrl) = 0;

protected:
    bool ProcessKeyEvent(KeyEventType eventType, Key key);

private:
    IKeyEventListener* m_pListener;

    bool m_bCapsLockActive;

    bool m_bShiftPressed;
    bool m_bCtrlPressed;
    bool m_bAltPressed;
};
