//
// ResourceLoader.cpp
//

#include "ResourceLoader.h"

wxIcon LoadIcon(const wxString& name)
{
#ifdef __WINDOWS__
    return wxIcon(name, wxBITMAP_TYPE_ICO_RESOURCE);
#elif defined __APPLE__
    wxIcon icon(name + ".icns", wxBITMAP_TYPE_ICON);
#else
    wxIcon icon(name + ".ico", wxBITMAP_TYPE_ICO);
#endif
}

wxIcon LoadClevyIcon()
{
    return LoadIcon("ClevyIcon");
}

wxIconArray LoadClevyIcons()
{
    wxIconArray icons;

    for (int i = 0; i <= 6; i++)
    {
        icons.Add(LoadIcon(wxString::Format("ClevyIcon%d", i)));
    }

    return icons;
}
