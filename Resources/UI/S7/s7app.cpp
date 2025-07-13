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
#include <wx/cmdline.h>

////@begin XPM images

////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( S7App )
////@end implement app

using namespace std;

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

    bool res = ParseCmdLine();
    if ( !res )
        return res;

    SetConfig();
    XS7 * appWindow = new XS7(nullptr);
    SetTopWindow(appWindow);
    appWindow->Show ( false );
    appWindow->Setup(m_config.get());
    appWindow->Show();
    
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

bool S7App::ParseCmdLine()
{
    wxCmdLineParser p;
    p.SetCmdLine ( wxApp::GetInstance()->argc, wxApp::GetInstance()->argv );
    p.SetSwitchChars ( _T ( "-" ) );
    p.AddOption ( _T ( "c" ), wxEmptyString, _ ( "Config file tag." ) );
    p.AddSwitch ( _T ( "v" ), wxEmptyString, _ ( "Show version and quit." ) );
    p.AddSwitch ( _T ( "h" ), wxEmptyString, _ ( "Show help and quit." ) );
    p.Parse ( false );
    if ( p.Found ( _T ( "c" ) ) )
    {
        p.Found ( _T ( "c" ), &m_configTag );
        return true;
    }
    if ( p.Found ( _T ( "h" ) ) )
    {
        p.Usage();
        return false; //Exit code is 255, not clean.
    }
    if ( p.Found ( _T ( "v" ) ) )
    {
        cout << ( _APPNAME_ + _ ( " - version " ) + to_string(_APPVERSION_) ) << endl;
        return false;
    }
    return true;
}

void S7App::SetConfig()
{
    const wxString configDir = wxFileConfig::GetLocalFile ( _APPNAME_, wxCONFIG_USE_SUBDIR ).GetPath();
    if ( !wxFileName::Exists ( configDir ) )
        wxFileName::Mkdir ( configDir );
    
    const wxString configBaseName = m_configTag.IsEmpty()
                                ? _APPNAME_
                                : _APPNAME_ + wxString("-") + m_configTag;
    m_config = std::make_unique<wxFileConfig>(_APPNAME_, _T("SET"), configBaseName,
                                              wxEmptyString, wxCONFIG_USE_SUBDIR);
}
