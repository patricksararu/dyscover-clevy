//
// Core.h
//

#pragma once

#include "Keyboard.h"

class App;
class Config;
class SoundPlayer;
class Speech;

class Core : public IKeyEventListener
{
public:
    Core(App*, Config*, Device*);
    virtual ~Core();

    virtual bool OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt) override;

    void OnClevyKeyboardConnected();
    void OnClevyKeyboardDisconnected();

    void UpdateAudioVolume();

private:
    App* m_pApp;
    Config* m_pConfig;
    Keyboard* m_pKeyboard;
    SoundPlayer* m_pSoundPlayer;
    Speech* m_pSpeech;

    std::string m_wordSpeechBuffer;
    std::string m_sentenceSpeechBuffer;

    bool m_bKeyboardConnected;
};
