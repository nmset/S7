/*
* File:   ConfigEditorPopup.cpp
* Author: Saleem EDAH-TALLY - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem EDAH-TALLY - © 2017
*
* Created on 4 mars 2017, 19:06
*/

#include "ConfigEditorPopup.h"
#include "MiscTools.h"

ConfigEditorPopup::ConfigEditorPopup ( wxWindow * parent, wxFileConfig* config )
{
  m_owner = parent;
  m_config = config;
}

ConfigEditorPopup::~ConfigEditorPopup() = default;

PopupTransientWindow* ConfigEditorPopup::CreatePopup()
{
  wxASSERT_MSG ( ( m_config != nullptr ),_T("CONFIG IS nullptr") );
  m_popup = new PopupTransientWindow ( m_owner );
  m_pan = new wxPanel ( m_popup );
  m_flxsz = new wxFlexGridSizer ( 0, 2, 0, 0 );
  m_pan->SetSizer ( m_flxsz );
  m_flxsz->AddGrowableCol ( 1 );
  m_popup->Bind ( wxEVT_DESTROY, &ConfigEditorPopup::OnPopupDestroy, this );
  return m_popup;
}

wxCheckBox* ConfigEditorPopup::AddCheckBox ( const wxString& label, const wxString& configPath )
{
  wxASSERT_MSG ( ( m_config != nullptr ),_T("CONFIG IS nullptr") );
  wxString * cPath = new wxString ( configPath );
  wxStaticText * lbl = new wxStaticText ( m_pan, wxID_ANY, label );
  m_flxsz->Add ( lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 0 );
  wxCheckBox * cb = new wxCheckBox ( m_pan, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
  m_flxsz->Add ( cb, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 0 );
  cb->SetValue ( m_config->ReadBool ( configPath, false ) );
  cb->SetClientData ( cPath );
  cb->Bind ( wxEVT_DESTROY, &ConfigEditorPopup::OnControlDestroy, this );
  return cb;
}

wxSpinCtrl* ConfigEditorPopup::AddSpinCtrl ( const wxString& label, const wxString& configPath )
{
  wxASSERT_MSG ( ( m_config != nullptr ),_T("CONFIG IS nullptr") );
  wxString * cPath = new wxString ( configPath );
  wxStaticText * lbl = new wxStaticText ( m_pan, wxID_ANY, label );
  m_flxsz->Add ( lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxGROW | wxALL, 0 );
  wxSpinCtrl * spn = new wxSpinCtrl ( m_pan, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -100, 100 );
  m_flxsz->Add ( spn, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 0 );
  spn->SetValue ( ( int ) m_config->ReadLong ( configPath, 0 ) );
  spn->SetClientData ( cPath );
  spn->Bind ( wxEVT_DESTROY, &ConfigEditorPopup::OnControlDestroy, this );
  return spn;
}

wxTextCtrl* ConfigEditorPopup::AddTextCtrl ( const wxString& label, const wxString& configPath )
{
  wxASSERT_MSG ( ( m_config != nullptr ),_T("CONFIG IS nullptr") );
  wxString * cPath = new wxString ( configPath );
  wxStaticText * lbl = new wxStaticText ( m_pan, wxID_ANY, label );
  m_flxsz->Add ( lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxGROW | wxALL, 0 );
  wxTextCtrl * txt = new wxTextCtrl ( m_pan, wxID_ANY );
  m_flxsz->Add ( txt, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 0 );
  txt->SetValue ( m_config->Read ( configPath, wxEmptyString ) );
  txt->SetClientData ( cPath );
  txt->Bind ( wxEVT_DESTROY, &ConfigEditorPopup::OnControlDestroy, this );
  return txt;
}

void ConfigEditorPopup::OnControlDestroy ( wxWindowDestroyEvent& evt )
{
  //cout << "ConfigEditorPopup::OnControlDestroy" << endl;
  evt.Skip();
  wxControl * ctrl = static_cast<wxControl*> ( evt.GetEventObject() );
  if ( !ctrl ) return;

  const wxString className ( ctrl->GetClassInfo()->GetClassName() );
  const wxString * cPath = static_cast<wxString*> ( ctrl->GetClientData() );
  if ( className ==_T("wxCheckBox") )
  {
    wxCheckBox * cb = static_cast<wxCheckBox*> ( evt.GetEventObject() );
    m_config->Write ( *cPath, cb->GetValue() );
  }
  if ( className ==_T("wxSpinCtrl") )
  {
    wxSpinCtrl * spn = static_cast<wxSpinCtrl*> ( evt.GetEventObject() );
    m_config->Write ( *cPath, spn->GetValue() );
  }
  if ( className ==_T("wxTextCtrl") )
  {
    wxTextCtrl * txt = static_cast<wxTextCtrl*> ( evt.GetEventObject() );
    if ( txt->IsEmpty() )
    {
      m_config->DeleteEntry ( *cPath, true );
    }
    else
    {
      m_config->Write ( *cPath, txt->GetValue() );
    }
  }

  m_config->Flush();
  delete cPath;
}

void ConfigEditorPopup::OnPopupDestroy ( wxWindowDestroyEvent& evt )
{
  //cout << "ConfigEditorPopup::OnPopupDestroy" << endl;
  evt.Skip();
}

void ConfigEditorPopup::ShowPopup()
{
  m_pan->GetSizer()->SetSizeHints ( m_pan );
  MiscTools::ShowTransientPopup ( m_popup, m_pan, m_pan->GetSize().GetWidth(), m_pan->GetSize().GetHeight() );
}


