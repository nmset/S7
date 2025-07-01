/////////////////////////////////////////////////////////////////////////////
// Name:        StampWidgets.cpp
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     dim. 06 juil. 2025 22:33:34
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
#include "wx/imaglist.h"
////@end includes

#include "StampWidgets.h"

////@begin XPM images
////@end XPM images


/*
 * StampWidgets type definition
 */

IMPLEMENT_DYNAMIC_CLASS( StampWidgets, wxPanel )


/*
 * StampWidgets event table definition
 */

BEGIN_EVENT_TABLE( StampWidgets, wxPanel )

////@begin StampWidgets event table entries
////@end StampWidgets event table entries

END_EVENT_TABLE()


/*
 * StampWidgets constructors
 */

StampWidgets::StampWidgets()
{
    Init();
}

StampWidgets::StampWidgets( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * StampWidgets creator
 */

bool StampWidgets::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin StampWidgets creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    Centre();
////@end StampWidgets creation
    return true;
}


/*
 * StampWidgets destructor
 */

StampWidgets::~StampWidgets()
{
////@begin StampWidgets destruction
////@end StampWidgets destruction
}


/*
 * Member initialisation
 */

void StampWidgets::Init()
{
////@begin StampWidgets member initialisation
    szNoteBookMain = NULL;
    szNoteBookButtons = NULL;
    btnAddStampWidget = NULL;
    btnDeleteStampWidget = NULL;
    nbStampWidgets = NULL;
////@end StampWidgets member initialisation
}


/*
 * Control creation for StampWidgets
 */

void StampWidgets::CreateControls()
{    
////@begin StampWidgets content construction
    StampWidgets* itemPanel1 = this;

    szNoteBookMain = new wxBoxSizer(wxHORIZONTAL);
    itemPanel1->SetSizer(szNoteBookMain);

    szNoteBookButtons = new wxBoxSizer(wxHORIZONTAL);
    szNoteBookMain->Add(szNoteBookButtons, 0, wxALIGN_TOP, 5);

    btnAddStampWidget = new wxButton( itemPanel1, ID_BUTTON_NB, _("+"), wxDefaultPosition, wxSize(30, -1), 0 );
    if (StampWidgets::ShowToolTips())
        btnAddStampWidget->SetToolTip(_("Add a stamp widget."));
    szNoteBookButtons->Add(btnAddStampWidget, 0, wxALIGN_CENTER_VERTICAL|wxFIXED_MINSIZE, 1);

    btnDeleteStampWidget = new wxButton( itemPanel1, ID_BUTTON_NB1, _("-"), wxDefaultPosition, wxSize(30, -1), 0 );
    if (StampWidgets::ShowToolTips())
        btnDeleteStampWidget->SetToolTip(_("Remove the selected stamp widget."));
    szNoteBookButtons->Add(btnDeleteStampWidget, 0, wxALIGN_CENTER_VERTICAL, 1);

    nbStampWidgets = new wxNotebook( itemPanel1, ID_NOTEBOOK_STAMPWIDGETS_, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    szNoteBookMain->Add(nbStampWidgets, 1, wxGROW|wxALL, 5);

////@end StampWidgets content construction
}


/*
 * Should we show tooltips?
 */

bool StampWidgets::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap StampWidgets::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin StampWidgets bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end StampWidgets bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon StampWidgets::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin StampWidgets icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end StampWidgets icon retrieval
}
