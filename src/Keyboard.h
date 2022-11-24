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

    virtual ~Keyboard() = default;

    virtual bool IsCapsLockActive() = 0;

    virtual void SendKeyEvent(KeyEventType eventType, Key key) = 0;

    void SendKeyStroke(Key key, bool shift, bool ctrl, bool alt);

    virtual std::string TranslateKeyStroke(Key key, bool shift, bool ctrl) = 0;

protected:
    explicit Keyboard(IKeyEventListener*);

    bool ProcessKeyEvent(KeyEventType eventType, Key key);

private:
    void Initialize();

    IKeyEventListener* m_pListener;

    bool m_bCapsLockActive;

    bool m_bShiftPressed;
    bool m_bCtrlPressed;
    bool m_bAltPressed;
};
