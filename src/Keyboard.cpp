//
// Keyboard.cpp
//

#include "Keyboard.h"
#include "KeyboardWindows.h"

Keyboard* Keyboard::create(PFNKBDHOOKCB pfnCallback, void* pUserData)
{
    Keyboard* pKeyboard;
#ifdef WIN32
    pKeyboard = new KeyboardWindows();
#endif
    pKeyboard->m_pfnCallback = pfnCallback;
    pKeyboard->m_pUserData = pUserData;
    return pKeyboard;
}

void Keyboard::InvokeCallback(KeyEventType keyEventType, int keyCode, int scanCode, int flags, int time)
{
    if (m_pfnCallback != nullptr)
    {
        (*m_pfnCallback)(keyEventType, keyCode, scanCode, flags, time, m_pUserData);
    }
}
