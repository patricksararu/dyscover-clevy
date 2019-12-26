//
// Core.cpp
//

#include "Core.h"
#include "KeyboardWindows.h"

Core::Core()
{
    m_pKeyboardWindows = new KeyboardWindows(this);
}

Core::~Core()
{
    delete m_pKeyboardWindows;
}

bool Core::OnKeyEvent(Key key, KeyEventType eventType, bool shift, bool ctrl, bool alt)
{
    KeyTranslation translation = TranslateKey(key, shift, ctrl, alt, Layout::DutchClassic);
    if (!translation.keystrokes.empty())
    {
        // Ignore KeyUp events
        if (eventType == KeyEventType::KeyDown)
        {
            for (KeyStroke ks : translation.keystrokes)
            {
                m_pKeyboardWindows->SendKeyPress(ks.key, ks.shift, ks.ctrl);
            }
        }

        // Supress this event
        return true;
    }

    return false;
}
