//
// Core.h
//

#pragma once

#include "Keyboard.h"

class Config;
class SoundPlayer;
class Speech;

class Core : public IKeyEventListener
{
public:
    Core(Config*);
    virtual ~Core();

    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt) override;

private:
    Config* m_pConfig;
    Keyboard* m_pKeyboard;
    SoundPlayer* m_pSoundPlayer;
    Speech* m_pSpeech;

    std::string m_speechBuffer;
};
