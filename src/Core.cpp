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

Core::Core(App* pApp, Config* pConfig)
{
    m_pApp = pApp;
    m_pConfig = pConfig;
    m_pKeyboard = Keyboard::Create(this);
    m_pSoundPlayer = new SoundPlayer();
    m_pSpeech = new Speech();
    m_pSpeech->Init(GetTTSDataPath(), "nl", "Ilse");
}

Core::~Core()
{
    m_pSpeech->Term();
    delete m_pSpeech;
    delete m_pSoundPlayer;
    delete m_pKeyboard;
}

bool Core::OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt)
{
    if (!m_pConfig->GetEnabled())  return false;

    if (key == Key::WinCmd && eventType == KeyEventType::KeyDown && m_pConfig->GetSelection())
    {
        // Send Ctrl+C
        m_pKeyboard->SendKeyStroke(Key::C, false, true);

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

                m_pSpeech->SetVolume(static_cast<float>(m_pConfig->GetVolume()));
                m_pSpeech->SetSpeed(static_cast<float>(m_pConfig->GetSpeed()));
                m_pSpeech->Speak(s.ToStdString());
            }

            wxTheClipboard->Close();
        }

        // Supress this event
        return true;
    }

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

    // Play sound
    if (eventType == KeyEventType::KeyDown)
    {
        // Is a single letter or a combination?
        if (translation.keystrokes.size() < 2)
        {
            if (m_pConfig->GetLettersAndNumbers())
            {
                m_pSoundPlayer->StopPlaying();
                m_pSoundPlayer->PlaySoundFile(translation.sound);
            }
        }
        else
        {
            if (m_pConfig->GetLetterCombinations())
            {
                m_pSoundPlayer->StopPlaying();
                m_pSoundPlayer->PlaySoundFile(translation.sound);
            }
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
        }
        else if (key == Key::Dot || ((key == Key::One || key == Key::Slash) && shift))
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
            if (!translation.keystrokes.empty())
            {
                for (KeyStroke ks : translation.keystrokes)
                {
                    std::string chars = m_pKeyboard->TranslateKeyStroke(ks.key, ks.shift, ks.ctrl);
                    m_wordSpeechBuffer.append(chars);
                    m_sentenceSpeechBuffer.append(chars);
                }
            }
            else
            {
                std::string chars = m_pKeyboard->TranslateKeyStroke(key, shift, ctrl);
                m_wordSpeechBuffer.append(chars);
                m_sentenceSpeechBuffer.append(chars);
            }
        }
    }

    return bSupressEvent;
}

void Core::OnClevyKeyboardConnected()
{
    m_pSoundPlayer->PlaySoundFile("dyscover_connect_positive_with_voice.wav");
}

void Core::OnClevyKeyboardDisconnected()
{
    m_pSoundPlayer->PlaySoundFile("dyscover_connect_negative_with_voice.wav");
}

void Core::UpdateAudioVolume()
{
    m_pSpeech->SetAudioVolume(m_pConfig->GetVolume() * 0xFFFF / 100);
}
