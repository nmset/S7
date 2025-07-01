// /*
//  * File:   XStampWidget.cpp
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 01 07 2025, 20:35
//  */

#include "XStampWidget.h"
#include "DefsStampWidget.h"
#include "StampWorker.h"
#include <wx/bmpbndl.h>

using namespace std;

IMPLEMENT_CLASS( XStampWidget, StampWidget )

XStampWidget::XStampWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: StampWidget(parent, id, pos, size, style)
{}

bool XStampWidget::Setup(wxConfig * config)
{
  m_config = config;
  if (!m_config)
    return false;

  const wxString fontDesc = m_config->Read("/Stamp/FontDesc", "");
  if (!fontDesc.IsEmpty())
  {
    wxFont font = wxNullFont;
    font.SetNativeFontInfo(fontDesc);
    if (font.IsOk())
    {
      fpkStamp->SetSelectedFont(font);
      fpkStamp->Update();
    }
  }
  long rgb = m_config->Read("/Stamp/ForegroundRGB", wxNOT_FOUND);
  if (rgb > wxNOT_FOUND)
  {
    wxColour foregroundColour = wxNullColour;
    foregroundColour.SetRGB((wxUint32) rgb);
    if (foregroundColour.IsOk())
    {
      cpkForegroundStamp->SetColour(foregroundColour);
    }
  }
  rgb = m_config->Read("/Stamp/BackgroundRGB", wxNOT_FOUND);
  if (rgb > wxNOT_FOUND)
  {
    wxColour backgroundColour = wxNullColour;
    backgroundColour.SetRGB((wxUint32) rgb);
    if (backgroundColour.IsOk())
    {
      cpkBackgroundStamp->SetColour(backgroundColour);
    }
  }
  int textRotationAngle = m_config->Read("/Stamp/RotationAngle", 45);
  sldTextRotationAngle->SetValue(textRotationAngle);
  bool transparency = m_config->ReadBool("/Stamp/Transparency", true);
  tglTransparent->SetValue(transparency);
  wxCommandEvent evt;
  evt.SetInt(transparency);
  OnTransparencyToggled(evt);

  fpkStamp->Bind(wxEVT_FONTPICKER_CHANGED, &XStampWidget::OnFontChanged, this);
  cpkForegroundStamp->Bind(wxEVT_COLOURPICKER_CHANGED, &XStampWidget::OnForegroundColourChanged, this);
  cpkBackgroundStamp->Bind(wxEVT_COLOURPICKER_CHANGED, &XStampWidget::OnBackgroundColourChanged, this);
  // *End* of any mouse and any keyboard interaction.
  sldTextRotationAngle->Bind(wxEVT_SCROLL_CHANGED, &XStampWidget::OnAngleSliderChanged, this);
  txtStamp->Bind(wxEVT_KEY_UP, &XStampWidget::OnTxtKeyPressed, this);
  tglTransparent->Bind(wxEVT_TOGGLEBUTTON, &XStampWidget::OnTransparencyToggled, this);

  wxArrayString stampLocations ({_("Centre"),
                        _("North"), _("South"), _("East"), _("West"),
                        _("North-east"), _("North-west"),
                        _("South-east"), _("South-west")});
  cmbStampLocation->Append(stampLocations);
  int location = m_config->Read("/Stamp/Location", wxNOT_FOUND);
  cmbStampLocation->Select((location >= cmbStampLocation->GetCount() || location < 0) ? 0 : location);
  cmbStampLocation->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &XStampWidget::OnLocationChanged, this);

  // A panel container is used to have its borders.
  m_sbmpPreview = new wxGenericStaticBitmap(panBitmapPreview, wxID_ANY, wxBitmapBundle(wxNullBitmap), wxDefaultPosition, panBitmapPreview->GetSize());
  szBitmapPreviewInPanel->Add(m_sbmpPreview, 0, wxGROW |  wxALL, 5);
  m_sbmpPreview->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);

  m_sbmpPreview->Bind(wxEVT_LEFT_UP, &XStampWidget::OnBitmapPreview, this);
  return true;
}

void XStampWidget::OnFontChanged(wxFontPickerEvent& evt)
{
  if (!m_config)
  {
    evt.Skip();
    return;
  }
  const wxString desc = evt.GetFont().GetNativeFontInfoDesc();
  m_config->Write("/Stamp/FontDesc", desc);
  m_config->Flush();
  
  evt.Skip();
}

void XStampWidget::OnForegroundColourChanged(wxColourPickerEvent& evt)
{
  if (!m_config)
  {
    evt.Skip();
    return;
  }
  const wxUint32 rgb = evt.GetColour().GetRGB();
  m_config->Write("/Stamp/ForegroundRGB", (long) rgb);
  m_config->Flush();
  
  evt.Skip();
}

void XStampWidget::OnBackgroundColourChanged(wxColourPickerEvent& evt)
{
  if (!m_config)
  {
    evt.Skip();
    return;
  }
  const wxUint32 rgb = evt.GetColour().GetRGB();
  m_config->Write("/Stamp/BackgroundRGB", (long) rgb);
  m_config->Flush();
  
  evt.Skip();
}

void XStampWidget::OnAngleSliderChanged(wxScrollEvent& evt)
{
  if (!m_config)
  {
    evt.Skip();
    return;
  }
  m_config->Write("/Stamp/RotationAngle", evt.GetInt());
  m_config->Flush();
  evt.Skip();
}

void XStampWidget::OnTxtKeyPressed(wxKeyEvent& evt)
{
  if (!m_config || !evt.ControlDown())
  {
    evt.Skip();
    return;
  }
  if (evt.GetKeyCode() == 'S')
  {
    if (txtStamp->IsEmpty())
    {
      evt.Skip();
      return;
    }
    m_config->Write("/Stamp/Text", txtStamp->GetValue());
    m_config->Flush();
  }
  else if (evt.GetKeyCode() == 'R')
  {
    wxString last;
    if (m_config->Read("/Stamp/Text", &last))
    {
      txtStamp->SetValue(last);
      txtStamp->SetSelection(last.Len(), last.Len());
    }
  }

  evt.Skip();
}

void XStampWidget::OnLocationChanged(wxCommandEvent& evt)
{
  if (!m_config)
  {
    evt.Skip();
    return;
  }
  m_config->Write("/Stamp/Location", evt.GetSelection());
  m_config->Flush();
  evt.Skip();
}

void XStampWidget::OnTransparencyToggled(wxCommandEvent& evt)
{
  lblBackgroundColour->Show(evt.GetInt() == 0);
  cpkBackgroundStamp->Show(evt.GetInt() == 0);
  if (GetSizer())
    GetSizer()->Layout();

  if (!m_config)
  {
    evt.Skip();
    return;
  }
  m_config->Write("/Stamp/Transparency", evt.GetInt());
  m_config->Flush();
  evt.Skip();
}


StampDescriptor * XStampWidget::GetStampDescriptor()
{
  m_descriptor.reset(nullptr);
  m_descriptor = std::make_unique<StampDescriptor> ();
  m_descriptor->text = txtStamp->GetValue();
  m_descriptor->font = fpkStamp->GetSelectedFont();
  m_descriptor->foregroundColour = cpkForegroundStamp->GetColour();
  m_descriptor->backgroundColour = cpkBackgroundStamp->GetColour();
  m_descriptor->rotationAngle = sldTextRotationAngle->GetValue();
  m_descriptor->location = cmbStampLocation->GetSelection();
  m_descriptor->transparent = tglTransparent->GetValue();
  // Not setting image, it is to be created.
  return m_descriptor.get();
}

void XStampWidget::OnBitmapPreview(wxMouseEvent& evt)
{
  StampDescriptor * descriptor = GetStampDescriptor();
  wxImage preview = StampWorker::CreateStamp(descriptor, 72);
  m_sbmpPreview->SetBitmap(wxBitmapBundle(preview));
  GetSizer()->Layout();
  evt.Skip();
}
