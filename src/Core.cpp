//
// Core.cpp
//

#include "Core.h"
#include "Keyboard.h"
#include "SoundPlayer.h"

Core::Core()
{
    m_pKeyboard = Keyboard::Create(this);
    m_pSoundPlayer = new SoundPlayer();
}

Core::~Core()
{
    delete m_pSoundPlayer;
    delete m_pKeyboard;
}

bool Core::OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt)
{
    KeyTranslation translation = TranslateKey(key, shift, ctrl, alt, Layout::DutchClassic);
    bool bSupressEvent = false;
    if (!translation.keystrokes.empty())
    {
        // Ignore KeyUp events
        if (eventType == KeyEventType::KeyDown)
        {
            for (KeyStroke ks : translation.keystrokes)
            {
                m_pKeyboard->SendKeyStroke(ks.key, ks.shift, ks.ctrl);
            }
        }

        // Supress this event
        bSupressEvent = true;
    }

    if (eventType == KeyEventType::KeyDown)
    {
        m_pSoundPlayer->StopPlaying();
        m_pSoundPlayer->PlaySoundFile(translation.sound);
    }

    return bSupressEvent;
}
