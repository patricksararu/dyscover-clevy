//
// Keyboard.cpp
//

#include "Keyboard.h"
#ifdef __WINDOWS__
#include "KeyboardWindows.h"
#elif __LINUX__
#include "KeyboardLinux.h"
#elif __APPLE__
#include "KeyboardMac.h"
#else
#error Unsupported platform
#endif

wxDEFINE_EVENT(wxEVT_KEY, wxThreadEvent);

Keyboard::~Keyboard()
{
}

Keyboard* Keyboard::create(IKeyTranslator* pTranslator, wxEvtHandler* pHandler, PFNKBDHOOKCB pfnCallback, void* pUserData)
{
    Keyboard* pKeyboard = nullptr;
#ifdef __WINDOWS__
    pKeyboard = new KeyboardWindows();
#elif __LINUX__
    pKeyboard = new KeyboardLinux();
#elif __APPLE__
    pKeyboard = new KeyboardMac();
#endif
    wxLogDebug("pKeyboard = %p", pKeyboard);
    pKeyboard->m_pHandler = pHandler;
    pKeyboard->m_pfnCallback = pfnCallback;
    pKeyboard->m_pUserData = pUserData;
    return pKeyboard;
}

void Keyboard::InvokeCallback(KeyEventType keyEventType, int keyCode, int scanCode, int flags, int time)
{
    wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_KEY));

    if (m_pfnCallback != nullptr)
    {
        (*m_pfnCallback)(keyEventType, keyCode, scanCode, flags, time, m_pUserData);
    }
}
