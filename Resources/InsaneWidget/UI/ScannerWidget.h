/////////////////////////////////////////////////////////////////////////////
// Name:        ScannerWidget.h
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     dim. 15 juin 2025 19:39:52
// RCS-ID:      
// Copyright:   Copyright Saleem EDAH-TALLY. All rights reserved.
// Licence:     CeCILL-C
/////////////////////////////////////////////////////////////////////////////

#ifndef _SCANNERWIDGET_H_
#define _SCANNERWIDGET_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SCANNERWIDGET 10000
#define wxID_STATIC_IMAGE_TYPE 10031
#define ID_COMBOBOX_IMAGE_TYPE 10007
#define ID_BUTTON_REFRESH_DEVICES 10006
#define wxID_STATIC_DEVICES 10029
#define ID_COMBOBOX_Devices 10001
#define wxID_STATIC_SOURCE 10028
#define ID_COMBOBOX_SOURCE 10002
#define wxID_STATIC_MODE 10027
#define ID_COMBOBOX_MODE 10003
#define wxID_STATIC_RESOLUTION 10030
#define ID_COMBOBOX_RESOLUTION 10004
#define ID_COMBOBOX 10005
#define SYMBOL_SCANNERWIDGET_STYLE wxTAB_TRAVERSAL
#define SYMBOL_SCANNERWIDGET_TITLE _("ScannerWidget")
#define SYMBOL_SCANNERWIDGET_IDNAME ID_SCANNERWIDGET
#define SYMBOL_SCANNERWIDGET_SIZE wxSize(400, 300)
#define SYMBOL_SCANNERWIDGET_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ScannerWidget class declaration
 */

class ScannerWidget: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( ScannerWidget )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ScannerWidget();
    ScannerWidget( wxWindow* parent, wxWindowID id = SYMBOL_SCANNERWIDGET_IDNAME, const wxPoint& pos = SYMBOL_SCANNERWIDGET_POSITION, const wxSize& size = SYMBOL_SCANNERWIDGET_SIZE, long style = SYMBOL_SCANNERWIDGET_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SCANNERWIDGET_IDNAME, const wxPoint& pos = SYMBOL_SCANNERWIDGET_POSITION, const wxSize& size = SYMBOL_SCANNERWIDGET_SIZE, long style = SYMBOL_SCANNERWIDGET_STYLE );

    /// Destructor
    ~ScannerWidget();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ScannerWidget event handler declarations

////@end ScannerWidget event handler declarations

////@begin ScannerWidget member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ScannerWidget member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ScannerWidget member variables
    wxStaticText* lblImageType;
    wxComboBox* cmbOutputType;
    wxButton* btnRefreshDevices;
    wxStaticText* lblDevices;
    wxComboBox* cmbDevices;
    wxStaticText* lblSource;
    wxComboBox* cmbSource;
    wxStaticText* lblMode;
    wxComboBox* cmbMode;
    wxStaticText* lblResolution;
    wxComboBox* cmbResolution;
    wxStaticText* lblPaperSize;
    wxComboBox* cmbPaperSize;
////@end ScannerWidget member variables
};

#endif
    // _SCANNERWIDGET_H_
