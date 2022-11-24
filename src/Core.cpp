//
// Core.cpp
//

#include <wx/clipbrd.h>
#include <wx/log.h>
#include <wx/time.h>

#include "App.h"
#include "Config.h"
#include "Core.h"
#include "Keyboard.h"
#include "ResourceLoader.h"
#include "SoundPlayer.h"
#include "Speech.h"
#include "VersionInfo.h"

Core::Core(App* pApp, Config* pConfig, Device* pDevice)
{
    m_pApp = pApp;
    m_pConfig = pConfig;
    m_pKeyboard = Keyboard::Create(this);
    m_pSoundPlayer = new SoundPlayer();
    m_pSpeech = new Speech();
    m_pSpeech->Init(GetTTSDataPath(), TTS_LANG, TTS_VOICE);
    m_pSpeech->SetVolume(RSTTS_VOLUME_MAX);

    m_bKeyboardConnected = pDevice != nullptr ? pDevice->IsClevyKeyboardPresent() : false;
}

Core::~Core()
{
    m_pSpeech->Term();
    delete m_pSpeech;
    delete m_pSoundPlayer;
    delete m_pKeyboard;
}

bool Core::OnKeyEvent(Key key, KeyEventType eventType, bool capsLock, bool shift, bool ctrl, bool alt)
{
#ifdef __LICENSING_FULL__
    if (!m_bKeyboardConnected)  return false;
#endif

    if (!m_pConfig->GetEnabled())  return false;

    if (key == Key::WinCmd && eventType == KeyEventType::KeyDown && m_pConfig->GetSelection())
    {
        // Send Ctrl+C
        m_pKeyboard->SendKeyStroke(Key::C, false, true, false);

        // Wait a while
        wxMilliSleep(25);

        // Read text from clipboard and pronounce it
        if (wxTheClipboard->Open())
        {
            if (wxTheClipboard->IsSupported(wxDF_TEXT))
            {
                wxTextDataObject tdo;
                wxTheClipboard->GetData(tdo);
                wxString s = tdo.GetText();

                m_pSpeech->SetSpeed(static_cast<float>(m_pConfig->GetSpeed()));
                m_pSpeech->Speak(s.ToStdString());
            }

            wxTheClipboard->Close();
        }

        // Supress this event
        return true;
    }

    KeyTranslation translation = TranslateKey(key, capsLock, shift, ctrl, alt, m_pConfig->GetLayout());

    // Send simulated key strokes
    if (eventType == KeyEventType::KeyDown)
    {
        for (KeyStroke ks : translation.keystrokes)
        {
            m_pKeyboard->SendKeyStroke(ks.key, ks.shift, ks.ctrl, ks.alt);
        }
    }

    // Play sound
    if (eventType == KeyEventType::KeyDown)
    {
        if (m_pConfig->GetLetters())
        {
            m_pSoundPlayer->StopPlaying();
            m_pSoundPlayer->PlaySoundFile(translation.sound);
        }
    }

    // Speech handling
    if (eventType == KeyEventType::KeyUp)
    {
        if (key == Key::Tab || key == Key::Space || key == Key::Enter)
        {
            if (!m_wordSpeechBuffer.empty() && m_pConfig->GetWords())
            {
                m_pSpeech->SetSpeed(static_cast<float>(m_pConfig->GetSpeed()));
                m_pSpeech->Speak(m_wordSpeechBuffer);
            }

            m_wordSpeechBuffer.clear();
            m_sentenceSpeechBuffer.push_back(' ');
        }
        else if (translation.speak_sentence)
        {
            m_pSpeech->SetSpeed(static_cast<float>(m_pConfig->GetSpeed()));

            if (!m_wordSpeechBuffer.empty() && m_pConfig->GetWords())
            {
                m_pSpeech->Speak(m_wordSpeechBuffer);
            }

            if (!m_sentenceSpeechBuffer.empty() && m_pConfig->GetSentences())
            {
                m_pSpeech->Speak(m_sentenceSpeechBuffer);
            }

            m_wordSpeechBuffer.clear();
            m_sentenceSpeechBuffer.clear();
        }
        else if (key == Key::Esc)
        {
            m_pSpeech->Stop();
        }
        else if (key == Key::Backspace)
        {
            if (!m_wordSpeechBuffer.empty())
            {
                m_wordSpeechBuffer.pop_back();
            }

            if (!m_sentenceSpeechBuffer.empty())
            {
                m_sentenceSpeechBuffer.pop_back();
            }
        }
        else
        {
            for (KeyStroke ks : translation.keystrokes)
            {
                std::string chars = m_pKeyboard->TranslateKeyStroke(ks.key, ks.shift, ks.ctrl);
                m_wordSpeechBuffer.append(chars);
                m_sentenceSpeechBuffer.append(chars);
            }
        }
    }

    // Supress all user keystrokes
    return true;
}

void Core::OnClevyKeyboardConnected()
{
    m_pSoundPlayer->PlaySoundFile("dyscover_connect_positive_with_voice.wav");

    m_bKeyboardConnected = true;
}

void Core::OnClevyKeyboardDisconnected()
{
    m_pSoundPlayer->PlaySoundFile("dyscover_connect_negative_with_voice.wav");

    m_bKeyboardConnected = false;
}
