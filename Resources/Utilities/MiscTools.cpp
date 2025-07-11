/*
* File:   MiscTools.cpp
* Author: Saleem EDAH-TALLY - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem EDAH-TALLY - © 2017
*
* Created on 4 mars 2017, 11:40
*/

#include "MiscTools.h"
#include <wx/notifmsg.h>
#include <wx/msgdlg.h>

MiscTools::MiscTools()
{
}

MiscTools::~MiscTools()
{
}

/*
 * Under this... thing called Wayland, GetPosition() returns (0, 0)!
 * Use "GDK_BACKEND=x11".
 */
void MiscTools::SaveSizePos(wxConfig* config, wxWindow* wind, const wxString& pathPrefix)
{
  wxASSERT_MSG ( ( config != nullptr ),_T("config IS nullptr") );
  wxASSERT_MSG ( ( wind != nullptr ),_T("wind IS nullptr") );
  config->Write ( pathPrefix +_T("/Width"), wind->GetSize().GetWidth() );
  config->Write ( pathPrefix +_T("/Height"), wind->GetSize().GetHeight() );
  config->Write ( pathPrefix +_T("/X"), wind->GetPosition().x );
  config->Write ( pathPrefix +_T("/Y"), wind->GetPosition().y );
  config->Flush();
}

void MiscTools::RestoreSizePos ( wxConfig* config, wxWindow* wind, const wxString& pathPrefix )
{
  wxASSERT_MSG ( ( config != nullptr ),_T("config IS nullptr") );
  wxASSERT_MSG ( ( wind != nullptr ),_T("wind IS nullptr") );
  long w = 0, h = 0;
  config->Read ( pathPrefix +_T("/Width"), &w );
  config->Read ( pathPrefix +_T("/Height"), &h );
  if ( w && h )
  {
    wind->SetSize ( w, h );
  }

  wxScreenDC dc;
  const int screenWidth = dc.GetSize().GetWidth();
  const int screenHeight = dc.GetSize().GetHeight();
  if (screenWidth && screenHeight)
  {
    long x = 0, y = 0;
    bool res = config->Read ( pathPrefix +_T("/X"), &x );
    res = config->Read ( pathPrefix +_T("/Y"), &y );
    if ( !res )
    {
      wind->Centre();
      return;
    }
    wind->SetPosition ( wxPoint ( x, y ) );
  }
}

void MiscTools::ShowTransientPopup ( wxPopupTransientWindow* p, wxWindow* content,
                                     const int width, const int height )
{
  wxASSERT_MSG ( ( p != nullptr ),_T("p IS nullptr") );
  wxASSERT_MSG ( ( content != nullptr ),_T("content IS nullptr") );
  wxBoxSizer * sz = new wxBoxSizer ( wxVERTICAL );
  p->SetSizer ( sz );
  int popupWidth = width;
  int popupHeight = height;
  if ( width < 1 )
    popupWidth = content->GetSize().GetWidth();
  if ( height < 1 )
    popupHeight = content->GetSize().GetHeight();
  p->SetSize ( wxSize ( popupWidth, popupHeight ) );

  sz->Add ( content, 0, wxGROW | wxALL );
  sz->Layout();
  /*
   * Under Wayland, screen size is invalid (0, 0) with wxScreenDC.
   * wxVideoMode obtained from wxApp doesn't help, it's even worse.
   * Using GDK_BACKEND="x11" fixes the placement issue with wxScreenDC,
   * but not with wxVideoMode.
   */
  wxScreenDC dc;
  const int screenWidth = dc.GetSize().GetWidth();
  const int screenHeight = dc.GetSize().GetHeight();
  const wxPoint mousePos = wxGetMousePosition();
  wxPoint dest ( mousePos.x, mousePos.y );
  if (screenWidth && screenHeight)
  {
    if ( mousePos.x > ( screenWidth - popupWidth ) ) dest.x = ( screenWidth - popupWidth );
    if ( mousePos.y > ( screenHeight - popupHeight ) ) dest.y = ( screenHeight - popupHeight );
    if ( popupWidth > screenWidth ) dest.x = 0;
    if ( popupHeight > screenHeight ) dest.y = 0;
  }
  p->SetPosition ( dest );
  p->Popup();
}

void MiscTools::MessageBox ( const wxString& msg, const bool notify )
{
  wxApp * app = static_cast<wxApp*> ( wxApp::GetInstance() );
  if ( !notify )
  {
    wxMessageBox ( msg, app->GetAppName(), wxOK, app->GetTopWindow() );
  }
  else
  {
    wxNotificationMessage notifMsg ( app->GetAppName(), msg, app->GetTopWindow() );
    notifMsg.Show();
  }
}

void MiscTools::AsyncMessageBox(const wxString& msg, const bool notify)
{
  wxTheApp->CallAfter([msg, notify] ()
    {
      MiscTools::MessageBox(msg, notify);
    }
  );
}

wxTextValidator* MiscTools::MakeFileNameValidator ( bool excludeSpace )
{
  wxTextValidator * tval = new wxTextValidator ( wxFILTER_EXCLUDE_CHAR_LIST );
  const wxString xcl = wxFileName::GetForbiddenChars();
  wxArrayString arr;
  for ( unsigned short i = 0; i < xcl.Length(); i++ )
  {
    arr.Add ( xcl.GetChar ( i ) );
  }
  if ( excludeSpace ) arr.Add (_T(" ") );
  arr.Add ( wxFileName::GetPathSeparator() );
  tval->SetExcludes ( arr );
  return tval;
}
