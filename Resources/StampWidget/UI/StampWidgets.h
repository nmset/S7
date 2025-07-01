/////////////////////////////////////////////////////////////////////////////
// Name:        StampWidgets.h
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     dim. 06 juil. 2025 22:33:34
// RCS-ID:      
// Copyright:   Copyright Saleem EDAH-TALLY. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _STAMPWIDGETS_H_
#define _STAMPWIDGETS_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxNotebook;
////@end forward declarations
#include <wx/panel.h>
#include <wx/button.h>
/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_STAMPWIDGETS 10000
#define ID_BUTTON_NB 10002
#define ID_BUTTON_NB1 10003
#define ID_NOTEBOOK_STAMPWIDGETS_ 10001
#define SYMBOL_STAMPWIDGETS_STYLE wxTAB_TRAVERSAL
#define SYMBOL_STAMPWIDGETS_TITLE _("StampWidgets")
#define SYMBOL_STAMPWIDGETS_IDNAME ID_STAMPWIDGETS
#define SYMBOL_STAMPWIDGETS_SIZE wxSize(400, 300)
#define SYMBOL_STAMPWIDGETS_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * StampWidgets class declaration
 */

class StampWidgets: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( StampWidgets )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    StampWidgets();
    StampWidgets( wxWindow* parent, wxWindowID id = SYMBOL_STAMPWIDGETS_IDNAME, const wxPoint& pos = SYMBOL_STAMPWIDGETS_POSITION, const wxSize& size = SYMBOL_STAMPWIDGETS_SIZE, long style = SYMBOL_STAMPWIDGETS_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_STAMPWIDGETS_IDNAME, const wxPoint& pos = SYMBOL_STAMPWIDGETS_POSITION, const wxSize& size = SYMBOL_STAMPWIDGETS_SIZE, long style = SYMBOL_STAMPWIDGETS_STYLE );

    /// Destructor
    ~StampWidgets();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin StampWidgets event handler declarations

////@end StampWidgets event handler declarations

////@begin StampWidgets member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end StampWidgets member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin StampWidgets member variables
    wxBoxSizer* szNoteBookMain;
    wxBoxSizer* szNoteBookButtons;
    wxButton* btnAddStampWidget;
    wxButton* btnDeleteStampWidget;
    wxNotebook* nbStampWidgets;
////@end StampWidgets member variables
};

#endif
    // _STAMPWIDGETS_H_
