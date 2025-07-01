// /*
//  * File:   XS7.h
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL
//  * Copyright Saleem Edah-Tally - © 2023
//  *
//  * Created on 18 06 2025, 22:42
//  */

#ifndef XS7_H
#define XS7_H

#include "s7.h"
#include <wx/wx.h>
#include <XInsaneWidget.h>
#include <fstream>

class XS7 : public S7
{
  DECLARE_DYNAMIC_CLASS( XS7 )
public:
  XS7(wxWindow* parent, wxWindowID id = SYMBOL_S7_IDNAME, const wxString& caption = SYMBOL_S7_TITLE,
      const wxPoint& pos = SYMBOL_S7_POSITION, const wxSize& size = SYMBOL_S7_SIZE, long style = SYMBOL_S7_STYLE );
  virtual ~XS7();
  bool ParseCmdLine();
  void Setup();

private:
  std::unique_ptr<wxConfig> m_config = nullptr;
  XInsaneWidget * m_insaneWidget = nullptr;

  /*
   * An optional tag for wxConfig files. This allows using different profiles
   * by specifying the -c command line option.
   */
  wxString m_configTag;

  void OnDpkRepositoryChange ( wxFileDirPickerEvent& evt );
  void OnDpkDoubleClick ( wxMouseEvent& evt );
  void OnAppKeyPressed(wxKeyEvent& evt);
  void OnNewDocLeftClick ( wxMouseEvent& evt ); // wxStaticText in m_insaneWidget.
  void OnAbout(wxMouseEvent& evt);
};

#endif // XS7_H
