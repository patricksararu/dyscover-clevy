//
// KeyboardLinux.h
//

#pragma once

#include "Keyboard.h"

class KeyboardLinux : public Keyboard
{
public:
    KeyboardLinux(IKeyEventListener* pListener);
    virtual ~KeyboardLinux();

    virtual void SendKeyEvent(KeyEventType eventType, Key key) override;
};
