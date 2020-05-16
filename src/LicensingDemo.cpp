//
// LicensingDemo.cpp
//

#include "App.h"
#include "Config.h"
#include "LicensingDemo.h"

enum
{
    ID_TIMER = wxID_HIGHEST + 1,
};

static const wxTimeSpan kDemoRunDuration = wxTimeSpan::Minutes(30);
static const wxTimeSpan kDemoLicenseValidityDuration = wxTimeSpan::Days(30);

DemoLicensing::DemoLicensing(Config* pConfig, IDemoLicensingListener* pListener)
{
    m_pConfig = pConfig;
    m_pListener = pListener;

    if (!m_pConfig->GetDemoExpired())
    {
        // Check if demo evaluation has expired
        wxDateTime demoStarted = m_pConfig->GetDemoStarted();
        wxDateTime now = wxDateTime::Now();
        if (!demoStarted.IsValid())
        {
            m_pConfig->SetDemoStarted(now);
        }
        else if (now > demoStarted + kDemoLicenseValidityDuration)
        {
            m_pConfig->SetDemoExpired(true);
        }
    }

    m_pTimer = new wxTimer(this, ID_TIMER);
    m_pTimer->StartOnce(kDemoRunDuration.GetMilliseconds().ToLong());
}

DemoLicensing::~DemoLicensing()
{
    delete m_pTimer;
}

bool DemoLicensing::HasDemoLicenseExpired()
{
    return m_pConfig->GetDemoExpired();
}

int DemoLicensing::GetDaysRemaining()
{
    wxDateTime start = m_pConfig->GetDemoStarted();
    wxDateTime now = wxDateTime::Now();
    wxTimeSpan span = start + kDemoLicenseValidityDuration - now;
    return span.GetDays();
}

void DemoLicensing::OnTimer(wxTimerEvent&)
{
    m_pListener->OnDemoTimeLimitExpired();
}

wxBEGIN_EVENT_TABLE(DemoLicensing, wxEvtHandler)
    EVT_TIMER(ID_TIMER, DemoLicensing::OnTimer)
wxEND_EVENT_TABLE()
