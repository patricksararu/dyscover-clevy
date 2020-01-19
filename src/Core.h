//
// Core.h
//

#pragma once

#include "Keyboard.h"

class SoundPlayer;

class Core : public IKeyEventListener
{
public:
    Core();
    virtual ~Core();

    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt) override;

private:
    Keyboard* m_pKeyboard;
    SoundPlayer* m_pSoundPlayer;
};
