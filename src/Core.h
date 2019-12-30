//
// Core.h
//

#pragma once

#include "Keyboard.h"

class Core : public IKeyEventListener
{
public:
    Core();
    ~Core();

    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt) override;

private:
    Keyboard* m_pKeyboard;
};
