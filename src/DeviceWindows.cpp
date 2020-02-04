//
// DeviceWindows.cpp
//

#include <wx/log.h>

#include "DeviceWindows.h"

DeviceWindows::DeviceWindows(IDeviceListener* pListener)
    : Device(pListener), wxFrame(nullptr, wxID_ANY, wxEmptyString)
{
    InitClevyKeyboardPresence();
}

DeviceWindows::~DeviceWindows()
{
}

WXLRESULT DeviceWindows::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    (void)wParam;
    (void)lParam;

    if (message == WM_DEVICECHANGE)
    {
        RefreshClevyKeyboardPresence();
    }

    return 0;
}

bool DeviceWindows::IsClevyKeyboardPresent()
{
    DEVINST hRootDevice;
    CONFIGRET cr = CM_Locate_DevNode(&hRootDevice, nullptr, CM_LOCATE_DEVINST_NORMAL);
    if (cr != CR_SUCCESS)
    {
        return false;
    }

    return DoesContainClevyKeyboard(hRootDevice);
}

bool DeviceWindows::DoesContainClevyKeyboard(DEVINST hDevice)
{
    while (true)
    {
        // Retrieve hardware ID
        TCHAR szHardwareId[1024] = { 0 };
        ULONG ulHardwareId = 1024;
        CONFIGRET cr = CM_Get_DevNode_Registry_Property(hDevice, CM_DRP_HARDWAREID, nullptr, szHardwareId, &ulHardwareId, 0);
        if (cr == CR_SUCCESS)
        {
            // Convert to upper case
            _tcsupr(szHardwareId);

            // Check if it matches known Clevy Keyboard hardware IDs
            if (_tcsstr(szHardwareId, TEXT("BTHENUM\\DEV_01000141")) != nullptr ||
                _tcsstr(szHardwareId, TEXT("USB\\VID_04B4&PID_0101")) != nullptr ||
                _tcsstr(szHardwareId, TEXT("USB\\VID_0CD3&PID_320F")) != nullptr)
            {
                return true;
            }
        }

        // Recursively iterate through child devices
        DEVINST hChildDevice;
        cr = CM_Get_Child(&hChildDevice, hDevice, 0);
        if (cr == CR_SUCCESS)
        {
            bool contains = DoesContainClevyKeyboard(hChildDevice);
            if (contains)
            {
                return true;
            }
        }

        // Move to next sibling device
        DEVINST hNextDevice;
        cr = CM_Get_Sibling(&hNextDevice, hDevice, 0);
        if (cr != CR_SUCCESS)
        {
            break;
        }
        hDevice = hNextDevice;
    }

    return false;
}
