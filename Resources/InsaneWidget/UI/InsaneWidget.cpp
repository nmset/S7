/////////////////////////////////////////////////////////////////////////////
// Name:        InsaneWidgetcpp.cpp
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     dim. 15 juin 2025 19:37:17
// RCS-ID:      
// Copyright:   Copyright Saleem EDAH-TALLY. All rights reserved.
// Licence:     CeCILL-C
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

#include "InsaneWidget.h"

////@begin XPM images
////@end XPM images


/*
 * InsaneWidget type definition
 */

IMPLEMENT_DYNAMIC_CLASS( InsaneWidget, wxPanel )


/*
 * InsaneWidget event table definition
 */

BEGIN_EVENT_TABLE( InsaneWidget, wxPanel )

////@begin InsaneWidget event table entries
////@end InsaneWidget event table entries

END_EVENT_TABLE()


/*
 * InsaneWidget constructors
 */

InsaneWidget::InsaneWidget()
{
    Init();
}

InsaneWidget::InsaneWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * InsaneWidget creator
 */

bool InsaneWidget::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin InsaneWidget creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end InsaneWidget creation
    return true;
}


/*
 * InsaneWidget destructor
 */

InsaneWidget::~InsaneWidget()
{
////@begin InsaneWidget destruction
////@end InsaneWidget destruction
}


/*
 * Member initialisation
 */

void InsaneWidget::Init()
{
////@begin InsaneWidget member initialisation
    lblNewDoc = NULL;
    txtNewDoc = NULL;
    btnScan = NULL;
////@end InsaneWidget member initialisation
}


/*
 * Control creation for InsaneWidget
 */

void InsaneWidget::CreateControls()
{    
////@begin InsaneWidget content construction
    InsaneWidget* itemPanel1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemPanel1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer1, 0, wxGROW|wxALL, 5);

    lblNewDoc = new wxStaticText( itemPanel1, ID_NewDoc_LBL, _("New"), wxDefaultPosition, wxDefaultSize, 0 );
    if (InsaneWidget::ShowToolTips())
        lblNewDoc->SetToolTip(_("'Right' click to define a scan project."));
    itemBoxSizer1->Add(lblNewDoc, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtNewDoc = new wxTextCtrl( itemPanel1, ID_NewDoc_TXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
    if (InsaneWidget::ShowToolTips())
        txtNewDoc->SetToolTip(_("Full path to destination file without the extension; it is determined by the output type."));
    itemBoxSizer1->Add(txtNewDoc, 1, wxGROW|wxALL, 5);

    btnScan = new wxButton( itemPanel1, ID_Scan_BTN, _("Scan"), wxDefaultPosition, wxDefaultSize, 0 );
    if (InsaneWidget::ShowToolTips())
        btnScan->SetToolTip(_("'Left click' to start the scan project.\n'Right click' to show the scanner widget."));
    itemBoxSizer1->Add(btnScan, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end InsaneWidget content construction
}


/*
 * Should we show tooltips?
 */

bool InsaneWidget::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap InsaneWidget::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin InsaneWidget bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end InsaneWidget bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon InsaneWidget::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin InsaneWidget icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end InsaneWidget icon retrieval
}
