//
// Core.h
//

#pragma once

#include "IKeyEventListener.h"

class KeyboardWindows;

class Core : public IKeyEventListener
{
public:
    Core();
    ~Core();

    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt) override;

private:
    KeyboardWindows* m_pKeyboardWindows;
};
