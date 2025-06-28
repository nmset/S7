/////////////////////////////////////////////////////////////////////////////
// Name:        ScannerWidget.cpp
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     dim. 15 juin 2025 19:39:52
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

#include "ScannerWidget.h"

////@begin XPM images
////@end XPM images


/*
 * ScannerWidget type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ScannerWidget, wxPanel )


/*
 * ScannerWidget event table definition
 */

BEGIN_EVENT_TABLE( ScannerWidget, wxPanel )

////@begin ScannerWidget event table entries
////@end ScannerWidget event table entries

END_EVENT_TABLE()


/*
 * ScannerWidget constructors
 */

ScannerWidget::ScannerWidget()
{
    Init();
}

ScannerWidget::ScannerWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * ScannerWidget creator
 */

bool ScannerWidget::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ScannerWidget creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ScannerWidget creation
    return true;
}


/*
 * ScannerWidget destructor
 */

ScannerWidget::~ScannerWidget()
{
////@begin ScannerWidget destruction
////@end ScannerWidget destruction
}


/*
 * Member initialisation
 */

void ScannerWidget::Init()
{
////@begin ScannerWidget member initialisation
    lblImageType = NULL;
    cmbOutputType = NULL;
    btnRefreshDevices = NULL;
    lblDevices = NULL;
    cmbDevices = NULL;
    lblSource = NULL;
    cmbSource = NULL;
    lblMode = NULL;
    cmbMode = NULL;
    lblResolution = NULL;
    cmbResolution = NULL;
    lblPaperSize = NULL;
    cmbPaperSize = NULL;
////@end ScannerWidget member initialisation
}


/*
 * Control creation for ScannerWidget
 */

void ScannerWidget::CreateControls()
{    
////@begin ScannerWidget content construction
    ScannerWidget* itemPanel1 = this;

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemPanel1->SetSizer(itemBoxSizer1);

    wxFlexGridSizer* itemFlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer1->Add(itemFlexGridSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    lblImageType = new wxStaticText( itemPanel1, wxID_STATIC_IMAGE_TYPE, _("Format:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer2->Add(lblImageType, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbOutputTypeStrings;
    cmbOutputType = new wxComboBox( itemPanel1, ID_COMBOBOX_IMAGE_TYPE, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbOutputTypeStrings, wxCB_READONLY );
    if (ScannerWidget::ShowToolTips())
        cmbOutputType->SetToolTip(_("Output document format"));
    itemFlexGridSizer2->Add(cmbOutputType, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer2->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnRefreshDevices = new wxButton( itemPanel1, ID_BUTTON_REFRESH_DEVICES, wxGetTranslation(wxString() + (wxChar) 0x21BB), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    if (ScannerWidget::ShowToolTips())
        btnRefreshDevices->SetToolTip(_("Refresh available devices"));
    itemBoxSizer5->Add(btnRefreshDevices, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblDevices = new wxStaticText( itemPanel1, wxID_STATIC_DEVICES, _("Devices:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(lblDevices, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbDevicesStrings;
    cmbDevices = new wxComboBox( itemPanel1, ID_COMBOBOX_Devices, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbDevicesStrings, wxCB_READONLY );
    if (ScannerWidget::ShowToolTips())
        cmbDevices->SetToolTip(_("Available devices"));
    itemFlexGridSizer2->Add(cmbDevices, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblSource = new wxStaticText( itemPanel1, wxID_STATIC_SOURCE, _("Source:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer2->Add(lblSource, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbSourceStrings;
    cmbSource = new wxComboBox( itemPanel1, ID_COMBOBOX_SOURCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbSourceStrings, wxCB_READONLY );
    if (ScannerWidget::ShowToolTips())
        cmbSource->SetToolTip(_("Scan source"));
    itemFlexGridSizer2->Add(cmbSource, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblMode = new wxStaticText( itemPanel1, wxID_STATIC_MODE, _("Mode:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer2->Add(lblMode, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbModeStrings;
    cmbMode = new wxComboBox( itemPanel1, ID_COMBOBOX_MODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbModeStrings, wxCB_READONLY );
    if (ScannerWidget::ShowToolTips())
        cmbMode->SetToolTip(_("Scan mode"));
    itemFlexGridSizer2->Add(cmbMode, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblResolution = new wxStaticText( itemPanel1, wxID_STATIC_RESOLUTION, _("Resolution:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer2->Add(lblResolution, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbResolutionStrings;
    cmbResolution = new wxComboBox( itemPanel1, ID_COMBOBOX_RESOLUTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbResolutionStrings, wxCB_READONLY );
    if (ScannerWidget::ShowToolTips())
        cmbResolution->SetToolTip(_("Scan resolution"));
    itemFlexGridSizer2->Add(cmbResolution, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblPaperSize = new wxStaticText( itemPanel1, wxID_STATIC, _("Paper size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer2->Add(lblPaperSize, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbPaperSizeStrings;
    cmbPaperSize = new wxComboBox( itemPanel1, ID_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbPaperSizeStrings, wxCB_READONLY );
    itemFlexGridSizer2->Add(cmbPaperSize, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer2->AddGrowableCol(1);

////@end ScannerWidget content construction
}


/*
 * Should we show tooltips?
 */

bool ScannerWidget::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap ScannerWidget::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ScannerWidget bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ScannerWidget bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon ScannerWidget::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ScannerWidget icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ScannerWidget icon retrieval
}
