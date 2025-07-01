// /*
//  * File:   XStampWidgets.cpp
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 06 07 2025, 22:41
//  */

#include "XStampWidgets.h"
#include <XStampWidget.h>
#include <DefsStampWidget.h>

IMPLEMENT_CLASS( XStampWidgets, StampWidgets )

XStampWidgets::XStampWidgets(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                             const wxSize& size, long style)
: StampWidgets(parent, id, pos, size, style)
{}

bool XStampWidgets::Setup(wxFileConfig* config)
{
  m_config = config;
  if (!m_config)
    return false;

  wxMouseEvent evt;
  AddStampWidget(evt);
  btnAddStampWidget->Bind(wxEVT_LEFT_UP, &XStampWidgets::AddStampWidget, this);
  btnDeleteStampWidget->Bind(wxEVT_LEFT_UP, &XStampWidgets::DeleteStampWidget, this);
  return true;
}

void XStampWidgets::AddStampWidget(wxMouseEvent& evt)
{
  XStampWidget * stampWidget = new XStampWidget(nbStampWidgets);
  stampWidget->Setup(m_config);
  stampWidget->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &XStampWidgets::OnStampLocationChanged, this);
  nbStampWidgets->AddPage(stampWidget, stampWidget->cmbStampLocation->GetStringSelection(), true);
  if (GetParent()->GetSizer())
    GetParent()->GetSizer()->Layout();
  
  evt.Skip();
}

void XStampWidgets::DeleteStampWidget(wxMouseEvent& evt)
{
  int selectedPage = nbStampWidgets->GetSelection();
  if (selectedPage == wxNOT_FOUND)
  {
    evt.Skip();
    return;
  }
  nbStampWidgets->DeletePage(selectedPage);
  evt.Skip();
}

std::vector<StampDescriptor*> * XStampWidgets::GetStampDescriptors()
{
  m_stampDescriptors.clear(); // Stored as unique_ptr in XStampWidget.
  for (uint i = 0; i < nbStampWidgets->GetPageCount(); i++)
  {
    XStampWidget * stampWidget = static_cast<XStampWidget*> (nbStampWidgets->GetPage(i));
    if (stampWidget)
    {
      m_stampDescriptors.push_back(stampWidget->GetStampDescriptor());
    }
  }
  return &m_stampDescriptors;
}

void XStampWidgets::OnStampLocationChanged(wxCommandEvent& evt)
{
  int selectedPage = nbStampWidgets->GetSelection();
  if (selectedPage == wxNOT_FOUND)
  {
    evt.Skip();
    return;
  }
  XStampWidget * stampWidget = static_cast<XStampWidget*> (nbStampWidgets->GetPage(selectedPage));
  if (!stampWidget)
  {
    evt.Skip();
    return;
  }
  nbStampWidgets->SetPageText(selectedPage, stampWidget->cmbStampLocation->GetStringSelection());
  evt.Skip();
}
