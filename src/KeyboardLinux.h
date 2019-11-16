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

    static constexpr KeyMapping s_keyMappings[] = {
        { Key::Esc, KEY_ESC },
        { Key::One, KEY_1 },
        { Key::Two, KEY_2 },
        { Key::Three, KEY_3 },
        { Key::Four, KEY_4 },
        { Key::Five, KEY_5 },
        { Key::Six, KEY_6 },
        { Key::Seven, KEY_7 },
        { Key::Eight, KEY_8 },
        { Key::Nine, KEY_9 },
        { Key::Zero, KEY_0 },
        { Key::Backspace, KEY_BACKSPACE },
        { Key::Tab, KEY_TAB },
    };
};
