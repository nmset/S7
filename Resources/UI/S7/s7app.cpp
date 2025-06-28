/////////////////////////////////////////////////////////////////////////////
// Name:        s7app.cpp
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     lun. 16 juin 2025 22:41:03
// RCS-ID:      
// Copyright:   Copyright Saleem EDAH-TALLY. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include <globals.h>
#include "s7app.h"
#include <XS7.h>

////@begin XPM images

////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( S7App )
////@end implement app


/*
 * S7App type definition
 */

IMPLEMENT_CLASS( S7App, wxApp )


/*
 * S7App event table definition
 */

BEGIN_EVENT_TABLE( S7App, wxApp )

////@begin S7App event table entries
////@end S7App event table entries

END_EVENT_TABLE()


/*
 * Constructor for S7App
 */

S7App::S7App()
{
    Init();
}


/*
 * Member initialisation
 */

void S7App::Init()
{
////@begin S7App member initialisation
////@end S7App member initialisation
}

/*
 * Initialisation for S7App
 */

bool S7App::OnInit()
{    
////@begin S7App initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
////@end S7App initialisation

#if wxUSE_LIBTIFF
    wxImage::AddHandler(new wxTIFFHandler);
#endif
#if wxUSE_PNM
    wxImage::AddHandler(new wxPNMHandler);
#endif

    m_locale.Init ( wxLANGUAGE_DEFAULT );
    m_locale.AddCatalog ( _APPNAME_ );
    
    SetAppName(_APPNAME_);
    wxTranslations * translations = wxTranslations::Get();
    if (translations)
    {
        translations->AddStdCatalog();
        translations->AddCatalog(_APPNAME_);
    }

    XS7 * appWindow = new XS7(nullptr);
    SetTopWindow(appWindow);
    appWindow->Show ( false );
    bool res = appWindow->ParseCmdLine();
    if ( res )
    {
        appWindow->Setup();
        appWindow->Show();
    }
    
    return res;
}


/*
 * Cleanup for S7App
 */

int S7App::OnExit()
{    
////@begin S7App cleanup
    return wxApp::OnExit();
////@end S7App cleanup
}

