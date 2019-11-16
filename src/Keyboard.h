//
// Keyboard.h
//

#pragma once

#include <wx/event.h>

#include "Keys.h"

enum class KeyEventType { KeyDown, KeyUp };

struct KeyMapping
{
    Key key;
    int code;
};

typedef void (*PFNKBDHOOKCB)(KeyEventType eventType, int keyCode, int scanCode, int flags, int time, void* pUser);

// TODO: Event should contain: KeyEventType, Key, bool shift, bool ctrl, bool alt, time_t timestamp

wxDECLARE_EVENT(wxEVT_KEY, wxThreadEvent);

class IKeyTranslator
{
    virtual Key* Translate(KeyEventType eventType, Key key, bool shift, bool ctrl, bool alt, time_t time) = 0;
};

class Keyboard
{
public:
    static Keyboard* create(IKeyTranslator*, wxEvtHandler*, PFNKBDHOOKCB, void* pUser);

    virtual ~Keyboard() = 0;

    //virtual void Send(KeyEventType, Key) = 0;

protected:
    //Keyboard();
    void InvokeCallback(KeyEventType keyEventType, int keyCode, int scanCode, int flags, int time);

private:
    wxEvtHandler* m_pHandler;
    PFNKBDHOOKCB m_pfnCallback;
    void* m_pUserData;
};
