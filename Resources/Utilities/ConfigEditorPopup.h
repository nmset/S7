/*
* File:   ConfigEditorPopup.h
* Author: Saleem EDAH-TALLY - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem EDAH-TALLY - © 2017
*
* Created on 4 mars 2017, 19:06
*/

#ifndef CONFIGEDITORPOPUP_H
#define CONFIGEDITORPOPUP_H

#include <wx/wx.h>
#include <wx/config.h>
#include "PopupTransientWindow.h"
#include <wx/spinctrl.h>

/**
 * Edits wxConfig keys using check boxes, text and spin controls, with supplied
 * paths. When a control is created, it reads the values. When it is destroyed,
 * it saves back the value.
 */
class ConfigEditorPopup
{
public:
  ConfigEditorPopup ( wxWindow * parent, wxConfig * config );
  virtual ~ConfigEditorPopup();
  PopupTransientWindow* CreatePopup();
  void ShowPopup();
  wxCheckBox* AddCheckBox ( const wxString& label, const wxString& configPath );
  wxSpinCtrl* AddSpinCtrl ( const wxString& label, const wxString& configPath );
  wxTextCtrl * AddTextCtrl ( const wxString& label, const wxString& configPath );

private:
  wxConfig * m_config = nullptr;
  wxWeakRef<wxWindow> m_owner = nullptr;
  wxFlexGridSizer * m_flxsz = nullptr;
  PopupTransientWindow * m_popup = nullptr;
  wxPanel * m_pan = nullptr;

  void OnControlDestroy ( wxWindowDestroyEvent& evt );
  void OnPopupDestroy ( wxWindowDestroyEvent& evt );
};

#endif /* CONFIGEDITORPOPUP_H */

