//
// KeyboardLinux.h
//

#pragma once

#include <linux/input.h>

#include <wx/log.h>
#include <wx/thread.h>

#include "Keyboard.h"

// TODO: Use std::thread instead of wxThread.
class KeyboardLinux : public Keyboard, public wxThread
{
public:
    KeyboardLinux();
    virtual ~KeyboardLinux();

private:
    virtual void* Entry() override;

    int m_eventfd;

    static Key KeyFromKeyCode(int keyCode);
    static int KeyCodeFromKey(Key key);
};
