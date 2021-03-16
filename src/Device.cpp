//
// Device.cpp
//

#include <wx/log.h>

#include "Device.h"

#ifdef WIN32
#include "DeviceWindows.h"
#else
#include "DeviceLinux.h"
#endif

Device* Device::Create(IDeviceListener* pListener)
{
#ifdef WIN32
    return new DeviceWindows(pListener);
#else
    return new DeviceLinux(pListener);
#endif
}

Device::Device(IDeviceListener* pListener)
{
    m_pListener = pListener;

    m_bIsClevyKeyboardPresent = false;
}

Device::~Device()
{
}

void Device::InitClevyKeyboardPresence()
{
    m_bIsClevyKeyboardPresent = IsClevyKeyboardPresent();

    wxLogDebug("Device::InitClevyKeyboardPresence()  presence = %d", m_bIsClevyKeyboardPresent);
}

void Device::RefreshClevyKeyboardPresence()
{
    // TODO: Move this logic to Core.
    bool bIsClevyKeyboardPresent = IsClevyKeyboardPresent();
    wxLogDebug("Device::RefreshClevyKeyboardPresence()  presence = %d -> %d", m_bIsClevyKeyboardPresent, bIsClevyKeyboardPresent);
    if (bIsClevyKeyboardPresent && !m_bIsClevyKeyboardPresent)
    {
        m_pListener->OnClevyKeyboardConnected();
    }
    else if (!bIsClevyKeyboardPresent && m_bIsClevyKeyboardPresent)
    {
        m_pListener->OnClevyKeyboardDisconnected();
    }
    m_bIsClevyKeyboardPresent = bIsClevyKeyboardPresent;
}
