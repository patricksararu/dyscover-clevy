//
// App.cpp
//

#include "App.h"
#include "TrayIcon.h"

bool App::OnInit()
{
    TrayIcon* pTrayIcon = new TrayIcon();

    return true;
}

wxIMPLEMENT_APP(App);
