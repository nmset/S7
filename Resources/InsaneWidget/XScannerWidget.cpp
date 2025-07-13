/*
* File:   XScannerWidget.cpp
* Author: Saleem Edah-Tally - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem Edah-Tally - © 2024
*
* Created on ?
*/

#include "XScannerWidget.h"
#include "XClientData.hpp"
#include "DefsInsaneWidget.h"
#include <libinsane/constants.h>
#include <fstream>
#include <vector>

using namespace std;

IMPLEMENT_CLASS( XScannerWidget, ScannerWidget )

XScannerWidget::XScannerWidget() : ScannerWidget()
{}

XScannerWidget::~XScannerWidget()
{}

XScannerWidget::XScannerWidget ( wxWindow* parent, wxWindowID id,
                                 const wxPoint& pos, const wxSize& size, long int style )
: ScannerWidget ( parent, id, pos, size, style )
{}

void XScannerWidget::Setup ( wxConfig* config, InsaneWorker * insaneWorker,
                             TimeredStatusBar * sb)
{
  wxASSERT_MSG ( ( config != nullptr ),_T("config IS nullptr") );
  wxASSERT_MSG ( ( insaneWorker != nullptr ),_T("insaneWorker IS nullptr") );
  m_config = config;
  m_sb = sb;
  m_insaneWorker = insaneWorker;

  UpdateExtensionsMap();
  if (m_insaneWorker)
    m_insaneWorkerEvh = m_insaneWorker->GetEventHandler();

  cmbOutputType->Append (Extensions[PDF]); // Use the file extension and the enum index without client data.
#if wxUSE_LIBPNG
  cmbOutputType->Append (Extensions[PNG]);
#endif
#if wxUSE_LIBJPEG
  cmbOutputType->Append (Extensions[JPEG]);
#endif
#if wxUSE_LIBTIFF
  cmbOutputType->Append (Extensions[TIFF]);
#endif
#if wxUSE_PNM
  cmbOutputType->Append (Extensions[PNM]);
#endif
  // Paper sizes handled by podofo.
  cmbPaperSize->Append(wxArrayString({"A0", "A1", "A2", "A3", "A4", "A5", "A6",
    "Letter", "Legal", "Tabloid"}));
  
  btnRefreshDevices->Bind ( wxEVT_COMMAND_BUTTON_CLICKED, &XScannerWidget::OnButtonRefreshDevices, this );
  cmbDevices->Bind ( wxEVT_COMMAND_COMBOBOX_SELECTED, &XScannerWidget::OnDeviceSelected, this );
  cmbOutputType->Bind ( wxEVT_COMMAND_COMBOBOX_SELECTED, &XScannerWidget::OnImageTypeSelected, this );
  cmbSource->Bind ( wxEVT_COMMAND_COMBOBOX_SELECTED, &XScannerWidget::OnSourceSelected, this );
  cmbMode->Bind ( wxEVT_COMMAND_COMBOBOX_SELECTED, &XScannerWidget::OnModeSelected, this );
  cmbResolution->Bind ( wxEVT_COMMAND_COMBOBOX_SELECTED, &XScannerWidget::OnResolutionSelected, this );
  cmbPaperSize->Bind ( wxEVT_COMMAND_COMBOBOX_SELECTED, &XScannerWidget::OnPaperSizeSelected, this );
  GetParent()->Bind ( wxEVT_SHOW, &XScannerWidget::OnActivated, this );

  wxString lastImageType;
  m_config->Read (_T("/Scanner/Last/OutputType"), &lastImageType );
  if ( cmbOutputType->FindString ( lastImageType, true ) != wxNOT_FOUND )
    cmbOutputType->SetStringSelection ( lastImageType );
  else
    cmbOutputType->Select(0);

  wxString lastPaperSize;
  m_config->Read (_T("/Scanner/Last/PaperSize"), &lastPaperSize );
  if ( cmbPaperSize->FindString ( lastPaperSize, true ) != wxNOT_FOUND )
    cmbPaperSize->SetStringSelection ( lastPaperSize );
  else
    cmbPaperSize->Select(4);
}

bool XScannerWidget::FindDevices ( bool async )
{
  if ( m_sb && !async )
    m_sb->SetTransientText (_("Searching for devices...") );
  if ( !m_insaneWorker->Init() )
  {
    const wxString msg = _("Could not initialise insane api.");
    cerr << msg << endl;
    if (m_insaneWorkerEvh)
      m_insaneWorkerEvh->OnInsaneError(msg.ToStdString());
    return false;
  }

  std::vector<InsaneWorker::DeviceDescriptor> devices = m_insaneWorker->GetDeviceDescriptors();

  CallAfter ( &XScannerWidget::ClearScannerItems );
  uint numberOfDevices = devices.size();

  for ( uint i = 0; i < numberOfDevices; i++ )
  {
    InsaneWorker::DeviceDescriptor device = devices.at(i);
    CallAfter ( &XScannerWidget::AppendScannerItem, device.GetLabel(), device.id );
  }

  if ( m_sb )
  {
    const wxString msg = wxVariant ( ( long ) numberOfDevices ).GetString() + _(" device(s) found.");
    // Should be but don't : it's as if the above code has not been run.
    // m_sb->CallAfter(&TimeredStatusBar::SetTransientText, msg, 3000);
    m_sb->SetTransientText(msg);
  }

  return true;
}

void XScannerWidget::AppendScannerItem ( const wxString& display, const wxString& clientData )
{
  cmbDevices->Append ( display, new XClientData ( clientData ) );
}

void XScannerWidget::ClearScannerItems()
{
  cmbDevices->Clear();
  cmbSource->Clear();
  cmbMode->Clear();
  cmbResolution->Clear();
}

void XScannerWidget::OnButtonRefreshDevices ( wxCommandEvent& evt )
{
  btnRefreshDevices->Enable ( false );
  m_insaneWorker->Init();
  FindDevices();
  evt.Skip();
  btnRefreshDevices->Enable ( true );
}

void XScannerWidget::OnDeviceSelected ( wxCommandEvent& evt )
{
  if ( !m_config || ( cmbDevices->GetSelection() == wxNOT_FOUND ) )
    return;

  int currentSelection = cmbDevices->GetCurrentSelection();
  if ( currentSelection == wxNOT_FOUND )
    return;
  XClientData * lastDevice = static_cast<XClientData*> ( cmbDevices->GetClientObject ( currentSelection ) );
  if ( !lastDevice )
    return;
  m_config->Write ( "/Scanner/Last/Device", lastDevice->GetData().GetString() );
  m_config->Flush();
  UpdateScannerOptions ( lastDevice->GetData().GetString() );
}

void XScannerWidget::OnImageTypeSelected ( wxCommandEvent& evt )
{
  if ( !m_config )
    return;

  m_config->Write ( "/Scanner/Last/OutputType", cmbOutputType->GetStringSelection() );
  m_config->Flush();
}

void XScannerWidget::OnSourceSelected ( wxCommandEvent& evt )
{
  if ( !m_config )
    return;
  const wxString deviceId = GetCurrentDeviceId();
  if ( wxIsEmpty ( deviceId ) )
    return;
  const wxString key =_T("/Scanner/") + deviceId +_T("/") + _T ( OPT_NAME_SOURCE );
  m_config->Write ( key, cmbSource->GetStringSelection() );
  m_config->Flush();
}

void XScannerWidget::OnModeSelected ( wxCommandEvent& evt )
{
  if ( !m_config )
    return;
  const wxString deviceId = GetCurrentDeviceId();
  if ( wxIsEmpty ( deviceId ) )
    return;
  const wxString key =_T("/Scanner/") + deviceId +_T("/") + _T ( OPT_NAME_MODE );
  m_config->Write ( key, cmbMode->GetStringSelection() );
  m_config->Flush();
}

void XScannerWidget::OnResolutionSelected ( wxCommandEvent& evt )
{
  if ( !m_config )
    return;
  const wxString deviceId = GetCurrentDeviceId();
  if ( wxIsEmpty ( deviceId ) )
    return;
  const wxString key =_T("/Scanner/") + deviceId +_T("/") + _T ( OPT_NAME_RESOLUTION );
  m_config->Write ( key, cmbResolution->GetStringSelection() );
  m_config->Flush();
}

void XScannerWidget::OnPaperSizeSelected(wxCommandEvent& evt)
{
  if ( !m_config )
    return;
  
  m_config->Write ( "/Scanner/Last/PaperSize", cmbPaperSize->GetStringSelection() );
  m_config->Flush();
}

void XScannerWidget::UpdateScannerOptions ( const wxString& deviceId )
{
  if ( wxIsEmpty ( deviceId ) )
    return;

  auto addListConstrainedOptions = [&] ( wxComboBox * cmb, const wxString& lastKey )
  {
    if (!cmb)
      return;
    cmb->Clear();
    vector<string> possible;
    const string defaultValue = m_insaneWorker->GetOptionPossibleValues(deviceId.ToStdString(), lastKey.ToStdString(), possible);
    for (string itemValue : possible)
    {
      cmb->Append(itemValue);
    }
    cmb->Enable(cmb->GetCount());
    if ( m_config )
    {
      wxString savedValue;
      const wxString key =_T("Scanner/") + deviceId +_T("/") + lastKey;
      m_config->Read ( key, &savedValue );
      if ( !savedValue.IsEmpty() )
      {
        cmb->SetStringSelection ( savedValue );
        return;
      }
    }
    cmb->SetStringSelection ( defaultValue );
  };

  /*
   * For a device with flatbed and ADF, there are multiple sources, each with a name.
   * The display name obtained here is different from the child devices of lis_item.
   * Each child item is a source. The root item must not be used for scanning
   * following the documentation. Fortunately, they are listed in the same order
   * with both enumerations.
   * Ex: Brother DCP-L8410CDW
   * 0 - FlatBed - flatbed
   * 1 - Automatic Document Feeder(left aligned) - feeder(left aligned)
   * 2 - Automatic Document Feeder(centrally aligned) - feeder(centrally aligned)
   * We must therefore select a source by its index.
   */
  addListConstrainedOptions(cmbSource, OPT_NAME_SOURCE);
  addListConstrainedOptions(cmbMode, OPT_NAME_MODE);
  addListConstrainedOptions(cmbResolution, OPT_NAME_RESOLUTION);
}


wxString XScannerWidget::GetCurrentDeviceId() const
{
  int currentSelection = cmbDevices->GetCurrentSelection();
  if ( currentSelection == wxNOT_FOUND )
    return wxEmptyString;
  XClientData * clientData = static_cast<XClientData*> ( cmbDevices->GetClientObject ( currentSelection ) );
  if ( !clientData )
    return wxEmptyString;
  return clientData->GetData().GetString();
}

void XScannerWidget::OnActivated ( wxShowEvent& evt )
{
  if ( evt.IsShown() && cmbDevices->GetCount() && cmbDevices->GetCurrentSelection() == wxNOT_FOUND )
  {
    if ( m_config )
    {
      wxString lastDevice;
      m_config->Read (_T("/Scanner/Last/Device"), &lastDevice );
      if ( lastDevice.IsEmpty() || !cmbDevices->GetCount() )
      {
        evt.Skip();
        return;
      }

      for ( uint i = 0; i < cmbDevices->GetCount(); i++ )
      {
        XClientData * clientData = static_cast<XClientData*> ( cmbDevices->GetClientObject ( i ) );
        if ( !clientData )
          continue;
        if ( clientData->GetData().GetString() == lastDevice )
        {
          cmbDevices->Select ( i );
          UpdateScannerOptions ( lastDevice );
          break;
        }
      }
    }
  }
  evt.Skip();
}

// ----------------------------------------------------------------------------
wxThread::ExitCode BackgroundScannerDiscovery::Entry()
{
  if (m_owner)
  {
    m_owner->FindDevices ( true );
    wxShowEvent evt;
    evt.SetShow ( true );
    m_owner->OnActivated ( evt );
    if (m_evh)
      m_evh->OnDone();
  }
  return ( wxThread::ExitCode ) 0;
}

// ----------------------------------------------------------------------------

void BackgroundScannerDiscoveryEvent::OnDone()
{}

