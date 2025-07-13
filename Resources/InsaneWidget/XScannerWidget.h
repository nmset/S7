/*
* File:   XScannerWidget.h
* Author: Saleem Edah-Tally - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem Edah-Tally - © 2024
*
* Created on ?
*/

#ifndef XSCANNERWIDGET_H
#define XSCANNERWIDGET_H

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/config.h>
#include <ScannerWidget.h>
#include "TimeredStatusBar.h"
#include "InsaneWorker.h"
#include <map>

class BackgroundScannerDiscoveryEvent;

/**
 * Find available scanner devices, USB or network attached.\n
 * Show the devices and their minimal properties in a popup.\n
 * Make the properties available through API.\n
 * Specify output file format and page sizes.\n
 * All selections are saved and restored using wxConfig.\n
 * Devices can be searched in a detached thread, typically on application
 * startup.\n
 */

class XScannerWidget : public ScannerWidget
{
  DECLARE_DYNAMIC_CLASS( XScannerWidget )
  friend class BackgroundScannerDiscovery;
public:
  XScannerWidget();
  virtual ~XScannerWidget();

  XScannerWidget ( wxWindow* parent, wxWindowID id = wxID_ANY,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = SYMBOL_SCANNERWIDGET_SIZE,
                   long int style = SYMBOL_SCANNERWIDGET_STYLE );

  void Setup ( wxConfig* config, InsaneWorker * insaneWorker,
               TimeredStatusBar * sb = nullptr );
  bool FindDevices ( bool async = false );
  wxString GetCurrentDeviceId() const;
  wxString GetScannerMode() const
  {
    return cmbMode->GetValue();
  }
  std::pair<int, wxString> GetScannerSource() const
  {
    return {cmbSource->GetCurrentSelection(), cmbSource->GetValue()};
  }
  wxString GetScannerResolution() const
  {
    return cmbResolution->GetValue();
  }
  uint GetScannerOutputType()
  {
    return cmbOutputType->GetCurrentSelection();
  }
  wxString GetPaperSize() const
  {
    return cmbPaperSize->GetValue();
  }

private:
  wxWeakRef<TimeredStatusBar> m_sb = nullptr;
  wxConfig * m_config = nullptr;

  InsaneWorker * m_insaneWorker;
  InsaneWorkerEvent * m_insaneWorkerEvh;

  void OnButtonRefreshDevices ( wxCommandEvent& evt );
  /*
   * For synchronous and asynchronous calls/
   */
  void AppendScannerItem ( const wxString& display, const wxString& clientData );
  void ClearScannerItems();

  void OnActivated ( wxShowEvent& evt );
  void OnImageTypeSelected ( wxCommandEvent& evt );
  void OnDeviceSelected ( wxCommandEvent& evt );
  void OnSourceSelected ( wxCommandEvent& evt );
  void OnModeSelected ( wxCommandEvent& evt );
  void OnResolutionSelected ( wxCommandEvent& evt );
  void OnPaperSizeSelected ( wxCommandEvent& evt );
  void UpdateScannerOptions ( const wxString& deviceId );
};

// Asynchronous device discovery.
class BackgroundScannerDiscovery : public wxThread
{
public:
  BackgroundScannerDiscovery ( XScannerWidget * owner, BackgroundScannerDiscoveryEvent * evh )
  {
    m_owner = owner;
    m_evh = evh;
  }
  virtual ~BackgroundScannerDiscovery() {};
  virtual ExitCode Entry();
private:
  XScannerWidget * m_owner = nullptr;
  BackgroundScannerDiscoveryEvent * m_evh = nullptr;
};

class BackgroundScannerDiscoveryEvent
{
public:
  virtual void OnDone();

};

#endif // XSCANNERWIDGET_H
