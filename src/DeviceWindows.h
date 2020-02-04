//
// DeviceWindows.h
//

#pragma once

#include <wx/frame.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cfgmgr32.h>

#include "Device.h"

class DeviceWindows : public Device, public wxFrame
{
public:
    DeviceWindows(IDeviceListener* pListener);
    virtual ~DeviceWindows();

    virtual bool IsClevyKeyboardPresent() override;

private:
    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam) override;

    bool DoesContainClevyKeyboard(DEVINST hDevice);
};
