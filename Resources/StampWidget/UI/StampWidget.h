/////////////////////////////////////////////////////////////////////////////
// Name:        StampWidget.h
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     mar. 01 juil. 2025 19:14:05
// RCS-ID:      
// Copyright:   Copyright Saleem EDAH-TALLY. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _STAMPWIDGET_H_
#define _STAMPWIDGET_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/clrpicker.h"
#include "wx/fontpicker.h"
#include "wx/tglbtn.h"
////@end includes
#include "wx/combobox.h"
#include "wx/stattext.h"
/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxFlexGridSizer;
class wxColourPickerCtrl;
class wxFontPickerCtrl;
class wxToggleButton;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_STAMPWIDGET 10000
#define ID_TEXTCTRL 10001
#define ID_STATIC_FOREGROUND_COLOUR 10008
#define ID_COLOURCTRL_FOREGROUND 10003
#define ID_STATIC_BACKGROUND_COLOUR 10009
#define ID_COLOURCTRL_BACKGROUND 10006
#define ID_STATIC_LOCATION 10010
#define ID_COMBOBOX 10005
#define ID_PANEL_BITMAP_PREVIEW 10011
#define ID_FONTCTRL 10002
#define ID_TOGGLE_TRANSPARENT 10007
#define ID_SLIDER 10004
#define SYMBOL_STAMPWIDGET_STYLE wxTAB_TRAVERSAL
#define SYMBOL_STAMPWIDGET_TITLE _("StampWidget")
#define SYMBOL_STAMPWIDGET_IDNAME ID_STAMPWIDGET
#define SYMBOL_STAMPWIDGET_SIZE wxSize(400, 300)
#define SYMBOL_STAMPWIDGET_POSITION wxDefaultPosition
////@end control identifiers

#include <wx/panel.h>
#include <wx/slider.h>

/*!
 * StampWidget class declaration
 */

class StampWidget: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( StampWidget )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    StampWidget();
    StampWidget( wxWindow* parent, wxWindowID id = SYMBOL_STAMPWIDGET_IDNAME, const wxPoint& pos = SYMBOL_STAMPWIDGET_POSITION, const wxSize& size = SYMBOL_STAMPWIDGET_SIZE, long style = SYMBOL_STAMPWIDGET_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_STAMPWIDGET_IDNAME, const wxPoint& pos = SYMBOL_STAMPWIDGET_POSITION, const wxSize& size = SYMBOL_STAMPWIDGET_SIZE, long style = SYMBOL_STAMPWIDGET_STYLE );

    /// Destructor
    ~StampWidget();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin StampWidget event handler declarations

////@end StampWidget event handler declarations

////@begin StampWidget member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end StampWidget member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin StampWidget member variables
    wxBoxSizer* szStampWidgetMain;
    wxTextCtrl* txtStamp;
    wxBoxSizer* szStampWidgetH0;
    wxFlexGridSizer* szStampWidgetFlexGrid;
    wxStaticText* lblForegroundColour;
    wxColourPickerCtrl* cpkForegroundStamp;
    wxStaticText* lblBackgroundColour;
    wxColourPickerCtrl* cpkBackgroundStamp;
    wxStaticText* lblLocation;
    wxComboBox* cmbStampLocation;
    wxPanel* panBitmapPreview;
    wxBoxSizer* szBitmapPreviewInPanel;
    wxBoxSizer* szStampWidgetH1;
    wxFontPickerCtrl* fpkStamp;
    wxToggleButton* tglTransparent;
    wxSlider* sldTextRotationAngle;
////@end StampWidget member variables
};

#endif
    // _STAMPWIDGET_H_
