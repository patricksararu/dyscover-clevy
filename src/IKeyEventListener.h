//
// IKeyEventListener.h
//

#pragma once

#include "Keys.h"

class IKeyEventListener
{
public:
    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt) = 0;
};
