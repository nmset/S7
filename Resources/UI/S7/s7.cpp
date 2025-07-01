/////////////////////////////////////////////////////////////////////////////
// Name:        s7.cpp
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     lun. 16 juin 2025 22:42:13
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

#include "s7.h"

////@begin XPM images
////@end XPM images


/*
 * S7 type definition
 */

IMPLEMENT_CLASS( S7, wxFrame )


/*
 * S7 event table definition
 */

BEGIN_EVENT_TABLE( S7, wxFrame )

////@begin S7 event table entries
////@end S7 event table entries

END_EVENT_TABLE()


/*
 * S7 constructors
 */

S7::S7()
{
    Init();
}

S7::S7( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * S7 creator
 */

bool S7::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin S7 creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end S7 creation
    return true;
}


/*
 * S7 destructor
 */

S7::~S7()
{
////@begin S7 destruction
////@end S7 destruction
}


/*
 * Member initialisation
 */

void S7::Init()
{
////@begin S7 member initialisation
    panMain = NULL;
    szMain = NULL;
    dpkDestination = NULL;
    txtBasename = NULL;
////@end S7 member initialisation
}


/*
 * Control creation for S7
 */

void S7::CreateControls()
{    
////@begin S7 content construction
    S7* itemFrame1 = this;

    panMain = new wxPanel( itemFrame1, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    panMain->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);

    szMain = new wxBoxSizer(wxVERTICAL);
    panMain->SetSizer(szMain);

    dpkDestination = new wxDirPickerCtrl( panMain, ID_DIRPICKERCTRL, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
    if (S7::ShowToolTips())
        dpkDestination->SetToolTip(_("Select a destination directory.\nDouble-click to go to the selected directory."));
    szMain->Add(dpkDestination, 0, wxGROW|wxALL, 5);

    txtBasename = new wxTextCtrl( panMain, ID_TEXTCTRL_S7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    txtBasename->SetHelpText(_("Basename"));
    if (S7::ShowToolTips())
        txtBasename->SetToolTip(_("Specify a destination file basename (without extension).\n\n'CTRL + click' for about information."));
    szMain->Add(txtBasename, 0, wxGROW|wxALL, 5);

////@end S7 content construction
}


/*
 * Should we show tooltips?
 */

bool S7::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap S7::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin S7 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end S7 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon S7::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin S7 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end S7 icon retrieval
}
