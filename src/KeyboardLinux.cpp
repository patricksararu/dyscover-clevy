//
// KeyboardLinux.cpp
//

#include "KeyboardLinux.h"

KeyboardLinux::KeyboardLinux(IKeyEventListener *pListener)
    : Keyboard(pListener)
{
}

KeyboardLinux::~KeyboardLinux()
{
}

bool KeyboardLinux::IsCapsLockActive()
{
    return false;
}

void KeyboardLinux::SendKeyEvent(KeyEventType eventType, Key key)
{
    (void)eventType;
    (void)key;
}

std::string KeyboardLinux::TranslateKeyStroke(Key key, bool shift, bool ctrl)
{
    (void)key;
    (void)shift;
    (void)ctrl;

    return std::string();
}
