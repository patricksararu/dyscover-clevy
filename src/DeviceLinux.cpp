//
// DeviceLinux.cpp
//

#include "DeviceLinux.h"

DeviceLinux::DeviceLinux(IDeviceListener *pListener)
    : Device(pListener)
{
}

bool DeviceLinux::IsClevyKeyboardPresent()
{
    return false;
}
