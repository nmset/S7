/////////////////////////////////////////////////////////////////////////////
// Name:        s7.h
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     lun. 16 juin 2025 22:42:13
// RCS-ID:      
// Copyright:   Copyright Saleem EDAH-TALLY. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _S7_H_
#define _S7_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/filepicker.h"
////@end includes
#include "wx/panel.h"
/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxDirPickerCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_S7 10000
#define ID_PANEL 10001
#define ID_DIRPICKERCTRL 10002
#define ID_TEXTCTRL_S7 10003
#define SYMBOL_S7_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_S7_TITLE _("S7")
#define SYMBOL_S7_IDNAME ID_S7
#define SYMBOL_S7_SIZE wxSize(400, 300)
#define SYMBOL_S7_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * S7 class declaration
 */

class S7: public wxFrame
{    
    DECLARE_CLASS( S7 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    S7();
    S7( wxWindow* parent, wxWindowID id = SYMBOL_S7_IDNAME, const wxString& caption = SYMBOL_S7_TITLE, const wxPoint& pos = SYMBOL_S7_POSITION, const wxSize& size = SYMBOL_S7_SIZE, long style = SYMBOL_S7_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_S7_IDNAME, const wxString& caption = SYMBOL_S7_TITLE, const wxPoint& pos = SYMBOL_S7_POSITION, const wxSize& size = SYMBOL_S7_SIZE, long style = SYMBOL_S7_STYLE );

    /// Destructor
    ~S7();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin S7 event handler declarations

////@end S7 event handler declarations

////@begin S7 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end S7 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin S7 member variables
    wxPanel* panMain;
    wxBoxSizer* szMain;
    wxDirPickerCtrl* dpkDestination;
    wxTextCtrl* txtBasename;
////@end S7 member variables
};

#endif
    // _S7_H_
