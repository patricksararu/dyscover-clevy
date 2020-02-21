//
// Core.h
//

#pragma once

#include "Device.h"
#include "Keyboard.h"

class Config;
class SoundPlayer;
class Speech;

class Core : public IDeviceListener, public IKeyEventListener
{
public:
    Core(Config*);
    virtual ~Core();

    virtual void OnClevyKeyboardConnected() override;
    virtual void OnClevyKeyboardDisconnected() override;

    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt) override;

private:
    Config* m_pConfig;
    Device* m_pDevice;
    Keyboard* m_pKeyboard;
    SoundPlayer* m_pSoundPlayer;
    Speech* m_pSpeech;

    std::string m_wordSpeechBuffer;
    std::string m_sentenceSpeechBuffer;
};
