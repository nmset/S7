/////////////////////////////////////////////////////////////////////////////
// Name:        s7app.h
// Purpose:     
// Author:      Saleem EDAH-TALLY
// Modified by: 
// Created:     lun. 16 juin 2025 22:41:03
// RCS-ID:      
// Copyright:   Copyright Saleem EDAH-TALLY. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _S7APP_H_
#define _S7APP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
////@end includes

#include "wx/config.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * S7App class declaration
 */

class S7App: public wxApp
{    
    DECLARE_CLASS( S7App )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    S7App();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin S7App event handler declarations
////@end S7App event handler declarations

////@begin S7App member function declarations
////@end S7App member function declarations

////@begin S7App member variables
////@end S7App member variables
private:
    wxLocale m_locale;
    std::unique_ptr<wxConfig> m_config = nullptr;
    /*
     * An optional tag for wxConfig files. This allows using different profiles
     * by specifying the -c command line option.
     */
    wxString m_configTag;

    bool ParseCmdLine();
    void SetConfig();
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(S7App)
////@end declare app

#endif
    // _S7APP_H_
