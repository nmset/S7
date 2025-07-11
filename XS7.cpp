// /*
//  * File:   XS7.cpp
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL
//  * Copyright Saleem Edah-Tally - © 2023
//  *
//  * Created on 18 06 2025, 22:42
//  */

#include "XS7.h"
#include "globals.h"
#include "TimeredStatusBar.h"
#include "MiscTools.h"
#include "page.xpm"
#include <wx/stdpaths.h>
#include <wx/cmdline.h>

using namespace std;

IMPLEMENT_CLASS( XS7, S7 )

XS7::XS7(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
: S7(parent, id, caption, pos, size, style)
{}

bool XS7::ParseCmdLine()
{
  wxCmdLineParser p;
  p.SetCmdLine ( wxApp::GetInstance()->argc, wxApp::GetInstance()->argv );
  p.SetSwitchChars ( _T ( "-" ) );
  p.AddOption ( _T ( "c" ), wxEmptyString, _ ( "Config file tag." ) );
  p.AddSwitch ( _T ( "v" ), wxEmptyString, _ ( "Show version and quit." ) );
  p.AddSwitch ( _T ( "h" ), wxEmptyString, _ ( "Show help and quit." ) );
  p.Parse ( false );
  if ( p.Found ( _T ( "c" ) ) )
  {
    p.Found ( _T ( "c" ), &m_configTag );
    return true;
  }
  if ( p.Found ( _T ( "h" ) ) )
  {
    p.Usage();
    return false; //Exit code is 255, not clean.
  }
  if ( p.Found ( _T ( "v" ) ) )
  {
    cout << ( _APPNAME_ + _ ( " - version " ) + to_string(_APPVERSION_) ) << endl;
    return false;
  }
  return true;
}

void XS7::Setup()
{
  const wxString configDir = wxFileConfig::GetLocalFile ( _APPNAME_, wxCONFIG_USE_SUBDIR ).GetPath();
  if ( !wxFileName::Exists ( configDir ) )
    wxFileName::Mkdir ( configDir );

  const wxString configBaseName = m_configTag.IsEmpty()
                        ? _APPNAME_
                        : _APPNAME_ + wxString("-") + m_configTag;
  m_config = std::make_unique<wxFileConfig>(_APPNAME_, _T("SET"), configBaseName,
                                            wxEmptyString, wxCONFIG_USE_SUBDIR);
  TimeredStatusBar * sb = new TimeredStatusBar(this);
  SetStatusBar(sb);
  
  m_insaneWidget = new XInsaneWidget(panMain, sb, m_config.get());
  m_insaneWidget->Setup();
  szMain->Insert(2, m_insaneWidget, 1, wxGROW | wxALL);
  
  dpkDestination->Bind ( wxEVT_DIRPICKER_CHANGED, &XS7::OnDpkRepositoryChange, this );
  dpkDestination->GetTextCtrl()->Bind ( wxEVT_LEFT_DCLICK, &XS7::OnDpkDoubleClick, this );
  Bind(wxEVT_CHAR_HOOK, &XS7::OnAppKeyPressed, this);
  m_insaneWidget->lblNewDoc->Bind ( wxEVT_LEFT_UP, &XS7::OnNewDocLeftClick, this );
  dpkDestination->SetPath(m_config->Read("/Docroot"));

  txtBasename->SetValidator(*MiscTools::MakeFileNameValidator(false));
  txtBasename->Bind(wxEVT_LEFT_UP, &XS7::OnAbout, this);
  MiscTools::RestoreSizePos(m_config.get(), this, wxString("/" + wxString(_APPNAME_)));

  S7::SetTitle(wxString(_APPNAME_) + " - version " +  to_string(_APPVERSION_));

  SetIcon(wxICON(page));
  wxString fixedTip = _("'Shift + left' click to generate a new destination file name.");
  fixedTip += _T("\n") + m_insaneWidget->lblNewDoc->GetToolTipText();
  m_insaneWidget->lblNewDoc->SetToolTip(fixedTip);
}

XS7::~XS7()
{
  if (m_config)
    MiscTools::SaveSizePos(m_config.get(), this, wxString("/") + _APPNAME_);
}

void XS7::OnDpkRepositoryChange ( wxFileDirPickerEvent& evt )
{
  m_config->Write ( _T ( "/DocRoot" ), dpkDestination->GetPath() );
  m_config->Flush();
  evt.Skip();
}

void XS7::OnDpkDoubleClick ( wxMouseEvent& evt )
{
  const wxString command = _T ( "xdg-open " ) + dpkDestination->GetPath();
  if ( wxExecute ( command ) == 0 )
  {
    const wxString msg = _( "Could not launch default file manager" );
    MiscTools::MessageBox( msg, true );
    cerr << msg << endl;
  }
  evt.Skip();
}

void XS7::OnAppKeyPressed(wxKeyEvent& evt)
{
  if (evt.ControlDown())
  {
    if (evt.GetKeyCode() == 'Q')
      Close();
  }
  if (evt.GetKeyCode() == WXK_ESCAPE)
    if (m_insaneWidget)
      m_insaneWidget->CancelScanning();
  evt.Skip();
}

void XS7::OnNewDocLeftClick ( wxMouseEvent& evt )
{
  if ( !evt.ShiftDown() || !m_insaneWidget->lblNewDoc->IsEnabled() )
  {
    evt.Skip();
    return;
  }
  if ( !dpkDestination->GetDirName().Exists() )
  {
    const wxString msg = _( "Invalid folder name." );
    MiscTools::MessageBox ( msg, true );
    cerr << msg << endl;
    evt.Skip();
    return;
  }
  if ( txtBasename->GetValue().IsEmpty() )
  {
    const wxString msg = _( "Invalid file basename." );
    MiscTools::MessageBox ( msg, true );
    cerr << msg << endl;
    evt.Skip();
    return;
  }
  const wxString filepath = dpkDestination->GetPath() + wxFileName::GetPathSeparator() + txtBasename->GetValue();
  
  m_insaneWidget->txtNewDoc->SetValue ( filepath );
  m_insaneWidget->ResetScanProject();
  evt.Skip();
}

void XS7::OnAbout(wxMouseEvent& evt)
{
  if (!evt.ControlDown())
  {
    evt.Skip();
    return;
  }
  const wxString msg = wxString(_APPNAME_) + _(" - version ") + to_string((_APPVERSION_))
      + _ (", using InsaneWidget.\n\n")
      + _ ( "Copyright: Saleem Edah-Tally [Surgeon] [Hobbyist developer]\n" )
      + _ ( "License: CeCILL/CeCILL-C per file header." );

  MiscTools::MessageBox(msg);
  evt.Skip();
}
