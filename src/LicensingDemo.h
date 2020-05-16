//
// LicensingDemo.h
//

#pragma once

#include <wx/timer.h>

class Config;

class IDemoLicensingListener
{
public:
    virtual void OnDemoTimeLimitExpired() = 0;
};

class DemoLicensing : public wxEvtHandler
{
public:
    DemoLicensing(Config*, IDemoLicensingListener* pListener);
    ~DemoLicensing();

    bool HasDemoLicenseExpired();

    int GetDaysRemaining();

private:
    wxDECLARE_EVENT_TABLE();

    Config* m_pConfig;

    IDemoLicensingListener* m_pListener;

    wxTimer* m_pTimer;

    void OnTimer(wxTimerEvent&);
};
