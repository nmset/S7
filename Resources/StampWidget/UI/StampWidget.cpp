/////////////////////////////////////////////////////////////////////////////
// Name:        StampWidget.cpp
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     mar. 01 juil. 2025 19:14:05
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

#include "StampWidget.h"

////@begin XPM images
////@end XPM images


/*
 * StampWidget type definition
 */

IMPLEMENT_DYNAMIC_CLASS( StampWidget, wxPanel )


/*
 * StampWidget event table definition
 */

BEGIN_EVENT_TABLE( StampWidget, wxPanel )

////@begin StampWidget event table entries
////@end StampWidget event table entries

END_EVENT_TABLE()


/*
 * StampWidget constructors
 */

StampWidget::StampWidget()
{
    Init();
}

StampWidget::StampWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * StampWidget creator
 */

bool StampWidget::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin StampWidget creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end StampWidget creation
    return true;
}


/*
 * StampWidget destructor
 */

StampWidget::~StampWidget()
{
////@begin StampWidget destruction
////@end StampWidget destruction
}


/*
 * Member initialisation
 */

void StampWidget::Init()
{
////@begin StampWidget member initialisation
    szStampWidgetMain = NULL;
    txtStamp = NULL;
    szStampWidgetH0 = NULL;
    szStampWidgetFlexGrid = NULL;
    lblForegroundColour = NULL;
    cpkForegroundStamp = NULL;
    lblBackgroundColour = NULL;
    cpkBackgroundStamp = NULL;
    lblLocation = NULL;
    cmbStampLocation = NULL;
    panBitmapPreview = NULL;
    szBitmapPreviewInPanel = NULL;
    szStampWidgetH1 = NULL;
    fpkStamp = NULL;
    tglTransparent = NULL;
    sldTextRotationAngle = NULL;
////@end StampWidget member initialisation
}


/*
 * Control creation for StampWidget
 */

void StampWidget::CreateControls()
{    
////@begin StampWidget content construction
    StampWidget* itemPanel1 = this;

    szStampWidgetMain = new wxBoxSizer(wxVERTICAL);
    itemPanel1->SetSizer(szStampWidgetMain);

    txtStamp = new wxTextCtrl( itemPanel1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    if (StampWidget::ShowToolTips())
        txtStamp->SetToolTip(_("Create a stamp with this text, which can be multiline.\n\nCTRL + S: save the current text.\nCTRL + R: restore the saved text."));
    szStampWidgetMain->Add(txtStamp, 0, wxGROW|wxALL, 5);

    szStampWidgetH0 = new wxBoxSizer(wxHORIZONTAL);
    szStampWidgetMain->Add(szStampWidgetH0, 0, wxALIGN_LEFT|wxALL, 5);

    szStampWidgetFlexGrid = new wxFlexGridSizer(0, 2, 0, 0);
    szStampWidgetH0->Add(szStampWidgetFlexGrid, 1, wxGROW|wxALL, 5);

    lblForegroundColour = new wxStaticText( itemPanel1, ID_STATIC_FOREGROUND_COLOUR, _("Foreground:"), wxDefaultPosition, wxDefaultSize, 0 );
    szStampWidgetFlexGrid->Add(lblForegroundColour, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    cpkForegroundStamp = new wxColourPickerCtrl( itemPanel1, ID_COLOURCTRL_FOREGROUND, wxColour(), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    if (StampWidget::ShowToolTips())
        cpkForegroundStamp->SetToolTip(_("Foreground colour of the text."));
    szStampWidgetFlexGrid->Add(cpkForegroundStamp, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblBackgroundColour = new wxStaticText( itemPanel1, ID_STATIC_BACKGROUND_COLOUR, _("Background:"), wxDefaultPosition, wxDefaultSize, 0 );
    szStampWidgetFlexGrid->Add(lblBackgroundColour, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    cpkBackgroundStamp = new wxColourPickerCtrl( itemPanel1, ID_COLOURCTRL_BACKGROUND, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    if (StampWidget::ShowToolTips())
        cpkBackgroundStamp->SetToolTip(_("Background colour of the text."));
    szStampWidgetFlexGrid->Add(cpkBackgroundStamp, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblLocation = new wxStaticText( itemPanel1, ID_STATIC_LOCATION, _("Location:"), wxDefaultPosition, wxDefaultSize, 0 );
    szStampWidgetFlexGrid->Add(lblLocation, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbStampLocationStrings;
    cmbStampLocation = new wxComboBox( itemPanel1, ID_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbStampLocationStrings, wxCB_READONLY );
    if (StampWidget::ShowToolTips())
        cmbStampLocation->SetToolTip(_("Location of the stamp on the output."));
    szStampWidgetFlexGrid->Add(cmbStampLocation, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    panBitmapPreview = new wxPanel( itemPanel1, ID_PANEL_BITMAP_PREVIEW, wxDefaultPosition, wxSize(200, 200), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    panBitmapPreview->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    if (StampWidget::ShowToolTips())
        panBitmapPreview->SetToolTip(_("Click to update the preview."));
    szStampWidgetH0->Add(panBitmapPreview, 0, wxALIGN_TOP|wxALL, 5);

    szBitmapPreviewInPanel = new wxBoxSizer(wxVERTICAL);
    panBitmapPreview->SetSizer(szBitmapPreviewInPanel);

    szStampWidgetH1 = new wxBoxSizer(wxHORIZONTAL);
    szStampWidgetMain->Add(szStampWidgetH1, 0, wxGROW|wxALL, 5);

    fpkStamp = new wxFontPickerCtrl( itemPanel1, ID_FONTCTRL, wxFont(), wxDefaultPosition, wxDefaultSize, wxFNTP_FONTDESC_AS_LABEL );
    if (StampWidget::ShowToolTips())
        fpkStamp->SetToolTip(_("Select the font of the stamp text."));
    szStampWidgetH1->Add(fpkStamp, 1, wxGROW|wxALL, 5);

    tglTransparent = new wxToggleButton( itemPanel1, ID_TOGGLE_TRANSPARENT, _("Transparent"), wxDefaultPosition, wxDefaultSize, 0 );
    tglTransparent->SetValue(true);
    if (StampWidget::ShowToolTips())
        tglTransparent->SetToolTip(_("Check for a completely transparent background and for a transparent text."));
    szStampWidgetH1->Add(tglTransparent, 0, wxGROW|wxALL, 5);

    sldTextRotationAngle = new wxSlider( itemPanel1, ID_SLIDER, 45, -180, 180, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS );
    if (StampWidget::ShowToolTips())
        sldTextRotationAngle->SetToolTip(_("Select the rotation angle of the stamp text."));
    szStampWidgetMain->Add(sldTextRotationAngle, 0, wxGROW|wxALL, 5);

////@end StampWidget content construction
}


/*
 * Should we show tooltips?
 */

bool StampWidget::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap StampWidget::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin StampWidget bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end StampWidget bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon StampWidget::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin StampWidget icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end StampWidget icon retrieval
}
