//
// Core.cpp
//

#include "Config.h"
#include "Core.h"
#include "Device.h"
#include "Keyboard.h"
#include "SoundPlayer.h"
#include "Speech.h"

Core::Core(Config* pConfig)
{
    m_pConfig = pConfig;
    m_pDevice = Device::Create(this);
    m_pKeyboard = Keyboard::Create(this);
    m_pSoundPlayer = new SoundPlayer();
    m_pSpeech = new Speech();
    m_pSpeech->Init("tts", "nl", "Ilse");
}

Core::~Core()
{
    m_pSpeech->Term();
    delete m_pSpeech;
    delete m_pSoundPlayer;
    delete m_pKeyboard;
    delete m_pDevice;
}

void Core::OnClevyKeyboardConnected()
{
    m_pSoundPlayer->PlaySoundFile("dyscover_connect_positive_with_voice.wav");
}

void Core::OnClevyKeyboardDisconnected()
{
    m_pSoundPlayer->PlaySoundFile("dyscover_connect_negative_with_voice.wav");
}

bool Core::OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt)
{
    if (m_pConfig->GetPaused())  return false;

    KeyTranslation translation = TranslateKey(key, shift, ctrl, alt, m_pConfig->GetLayout());
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

    if (eventType == KeyEventType::KeyDown && m_pConfig->GetSound() && m_pConfig->GetSounds())
    {
        m_pSoundPlayer->StopPlaying();
        m_pSoundPlayer->PlaySoundFile(translation.sound);
    }

    // Speech handling
    if (eventType == KeyEventType::KeyUp)
    {
        if (key == Key::Tab || key == Key::Space || key == Key::Enter)
        {
            if (!m_speechBuffer.empty() && m_pConfig->GetSound() && m_pConfig->GetTTS() && m_pConfig->GetWord())
            {
                m_pSpeech->SetVolume(static_cast<float>(m_pConfig->GetVolume()));
                m_pSpeech->SetSpeed(static_cast<float>(m_pConfig->GetSpeed()));
                m_pSpeech->Speak(m_speechBuffer);
            }

            m_speechBuffer.clear();
        }
        else if (key == Key::Backspace)
        {
            if (!m_speechBuffer.empty())
            {
                m_speechBuffer.pop_back();
            }
        }
        else
        {
            if (!translation.keystrokes.empty())
            {
                for (KeyStroke ks : translation.keystrokes)
                {
                    m_speechBuffer.append(m_pKeyboard->TranslateKeyStroke(ks.key, ks.shift, ks.ctrl));
                }
            }
            else
            {
                m_speechBuffer.append(m_pKeyboard->TranslateKeyStroke(key, shift, ctrl));
            }
        }
    }

    return bSupressEvent;
}
