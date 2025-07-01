// /*
//  * File:   XStampWidget.h
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 01 07 2025, 20:35
//  */

#ifndef XSTAMPWIDGET_H
#define XSTAMPWIDGET_H

#include "StampWidget.h"
#include <wx/wx.h>
#include <wx/config.h>
#include <wx/generic/statbmpg.h>

struct StampDescriptor;

/**
 * This widget collects inputs for the creation of a stamp on scan pages.\n
 *  - text
 *  - font
 *  - foreground colour
 *  - background colour
 *  - angle of rotation
 *  - transparency.
 */
class XStampWidget : public StampWidget
{
  DECLARE_DYNAMIC_CLASS( XStampWidget )
public:
  XStampWidget( wxWindow* parent, wxWindowID id = SYMBOL_STAMPWIDGET_IDNAME, const wxPoint& pos = SYMBOL_STAMPWIDGET_POSITION, const wxSize& size = SYMBOL_STAMPWIDGET_SIZE, long style = SYMBOL_STAMPWIDGET_STYLE );

  bool Setup(wxConfig * config);
  StampDescriptor * GetStampDescriptor();
private:
  wxConfig * m_config;
  std::unique_ptr<StampDescriptor> m_descriptor;
  wxGenericStaticBitmap * m_sbmpPreview;

  void OnFontChanged(wxFontPickerEvent& evt);
  void OnForegroundColourChanged(wxColourPickerEvent& evt);
  void OnBackgroundColourChanged(wxColourPickerEvent& evt);
  void OnAngleSliderChanged(wxScrollEvent& evt);
  void OnTxtKeyPressed ( wxKeyEvent& evt );
  void OnLocationChanged(wxCommandEvent& evt);
  void OnTransparencyToggled(wxCommandEvent& evt);
  void OnBitmapPreview(wxMouseEvent& evt);
};

#endif // XSTAMPWIDGET_H
