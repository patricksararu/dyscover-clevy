//
// Keyboard.cpp
//

#include "Keyboard.h"

#ifdef WIN32
#include "KeyboardWindows.h"
#else
#error Unsupported platform
#endif

Keyboard* Keyboard::Create(IKeyEventListener* pListener)
{
#ifdef WIN32
    return new KeyboardWindows(pListener);
#else
    return nullptr;
#endif
}

Keyboard::Keyboard(IKeyEventListener* pListener)
{
    m_pListener = pListener;

    m_bShiftPressed = false;
    m_bCtrlPressed = false;
    m_bAltPressed = false;
}

Keyboard::~Keyboard()
{
}

void Keyboard::SendKeyStroke(Key key, bool shift, bool ctrl)
{
    if (m_bShiftPressed && !shift)
    {
        // Release Shift
        SendKeyEvent(KeyEventType::KeyUp, Key::Shift);
    }
    else if (!m_bShiftPressed && shift)
    {
        // Press Shift
        SendKeyEvent(KeyEventType::KeyDown, Key::Shift);
    }

    if (m_bCtrlPressed && !ctrl)
    {
        // Release Ctrl
        SendKeyEvent(KeyEventType::KeyUp, Key::Ctrl);
    }
    else if (!m_bCtrlPressed && ctrl)
    {
        // Press Ctrl
        SendKeyEvent(KeyEventType::KeyDown, Key::Ctrl);
    }

    SendKeyEvent(KeyEventType::KeyDown, key);
    SendKeyEvent(KeyEventType::KeyUp, key);

    if (m_bShiftPressed && !shift)
    {
        // Re-press Shift
        SendKeyEvent(KeyEventType::KeyDown, Key::Shift);
    }
    else if (!m_bShiftPressed && shift)
    {
        // Release Shift
        SendKeyEvent(KeyEventType::KeyUp, Key::Shift);
    }

    if (m_bCtrlPressed && !ctrl)
    {
        // Re-press Ctrl
        SendKeyEvent(KeyEventType::KeyDown, Key::Ctrl);
    }
    else if (!m_bCtrlPressed && ctrl)
    {
        // Release Ctrl
        SendKeyEvent(KeyEventType::KeyUp, Key::Ctrl);
    }
}

bool Keyboard::ProcessKeyEvent(KeyEventType eventType, Key key)
{
    if (key == Key::Shift)
    {
        m_bShiftPressed = eventType == KeyEventType::KeyDown;
    }

    if (key == Key::Ctrl)
    {
        m_bCtrlPressed = eventType == KeyEventType::KeyDown;
    }

    if (key == Key::Alt)
    {
        m_bAltPressed = eventType == KeyEventType::KeyDown;
    }

    if (key != Key::Unknown)
    {
        return m_pListener->OnKeyEvent(key, eventType, m_bShiftPressed, m_bCtrlPressed, m_bAltPressed);
    }

    return false;
}
