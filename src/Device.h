//
// Device.h
//

#pragma once

class IDeviceListener
{
public:
    virtual void OnClevyKeyboardConnected() = 0;
    virtual void OnClevyKeyboardDisconnected() = 0;
};

class Device
{
public:
    static Device* Create(IDeviceListener*);

    Device(IDeviceListener*);
    virtual ~Device();

    virtual bool IsClevyKeyboardPresent() = 0;

protected:
    void InitClevyKeyboardPresence();
    void RefreshClevyKeyboardPresence();

private:
    IDeviceListener* m_pListener;

    bool m_bIsClevyKeyboardPresent;
};
