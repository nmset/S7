/////////////////////////////////////////////////////////////////////////////
// Name:        InsaneWidget.h
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     dim. 15 juin 2025 19:37:17
// RCS-ID:      
// Copyright:   Copyright Saleem EDAH-TALLY. All rights reserved.
// Licence:     CeCILL-C
/////////////////////////////////////////////////////////////////////////////

#ifndef _INSANEWIDGET_H_
#define _INSANEWIDGET_H_


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
#define ID_INSANEWIDGET 10000
#define ID_NewDoc_LBL 10014
#define ID_NewDoc_TXT 10005
#define ID_Scan_BTN 10006
#define SYMBOL_INSANEWIDGET_STYLE wxTAB_TRAVERSAL
#define SYMBOL_INSANEWIDGET_TITLE _("InsaneWidget")
#define SYMBOL_INSANEWIDGET_IDNAME ID_INSANEWIDGET
#define SYMBOL_INSANEWIDGET_SIZE wxSize(400, 300)
#define SYMBOL_INSANEWIDGET_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * InsaneWidget class declaration
 */

class InsaneWidget: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( InsaneWidget )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    InsaneWidget();
    InsaneWidget( wxWindow* parent, wxWindowID id = SYMBOL_INSANEWIDGET_IDNAME, const wxPoint& pos = SYMBOL_INSANEWIDGET_POSITION, const wxSize& size = SYMBOL_INSANEWIDGET_SIZE, long style = SYMBOL_INSANEWIDGET_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_INSANEWIDGET_IDNAME, const wxPoint& pos = SYMBOL_INSANEWIDGET_POSITION, const wxSize& size = SYMBOL_INSANEWIDGET_SIZE, long style = SYMBOL_INSANEWIDGET_STYLE );

    /// Destructor
    ~InsaneWidget();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin InsaneWidget event handler declarations

////@end InsaneWidget event handler declarations

////@begin InsaneWidget member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end InsaneWidget member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin InsaneWidget member variables
    wxStaticText* lblNewDoc;
    wxTextCtrl* txtNewDoc;
    wxButton* btnScan;
////@end InsaneWidget member variables
};

#endif
    // _INSANEWIDGET_H_
