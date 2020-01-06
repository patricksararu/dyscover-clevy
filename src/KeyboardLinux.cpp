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

void KeyboardLinux::SendKeyEvent(KeyEventType eventType, Key key)
{
    (void)eventType;
    (void)key;
}
