// /*
//  * File:   XStampWidgets.h
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 06 07 2025, 22:41
//  */

#ifndef XSTAMPWIDGETS_H
#define XSTAMPWIDGETS_H

#include "StampWidgets.h"
#include <wx/wx.h>
#include <wx/config.h>
#include <vector>
#include <memory>

struct StampDescriptor;

class XStampWidgets : public StampWidgets
{
  DECLARE_DYNAMIC_CLASS( XStampWidgets )
public:
  XStampWidgets(wxWindow* parent, wxWindowID id = SYMBOL_STAMPWIDGETS_IDNAME,
                const wxPoint& pos = SYMBOL_STAMPWIDGETS_POSITION,
                const wxSize& size = SYMBOL_STAMPWIDGETS_SIZE,
                long style = SYMBOL_STAMPWIDGETS_STYLE);
  bool Setup(wxConfig * config);
  std::vector<StampDescriptor*> * GetStampDescriptors();

private:
  wxConfig * m_config;
  std::vector<StampDescriptor*> m_stampDescriptors;

  void AddStampWidget(wxMouseEvent& evt);
  void DeleteStampWidget(wxMouseEvent& evt);
  void OnStampLocationChanged(wxCommandEvent& evt);
};

#endif // XSTAMPWIDGETS_H
