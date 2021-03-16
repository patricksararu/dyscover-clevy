//
// DeviceLinux.h
//

#pragma once

#include "Device.h"

class DeviceLinux : public Device
{
public:
    DeviceLinux(IDeviceListener* pListener);

    virtual bool IsClevyKeyboardPresent() override;
};
