//
// Core.h
//

#pragma once

#include "Keyboard.h"

class SoundPlayer;
class Speech;

class Core : public IKeyEventListener
{
public:
    Core();
    virtual ~Core();

    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt) override;

private:
    Keyboard* m_pKeyboard;
    SoundPlayer* m_pSoundPlayer;
    Speech* m_pSpeech;

    std::string m_speechBuffer;
};
