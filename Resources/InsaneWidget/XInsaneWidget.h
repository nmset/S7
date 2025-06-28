/*
* File:   XInsaneWidget.h
* Author: Saleem Edah-Tally - nmset@yandex.com
* License : CeCILL-C
* Copyright Saleem Edah-Tally - © 2025
*
* Created on 15 06 2025, 21:24
*/

#ifndef XINSANEWIDGET_H
#define XINSANEWIDGET_H

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/popupwin.h>
#include <InsaneWidget.h>
#include <InsaneWorker.h>
#include <XScannerWidget.h>
#include <TimeredStatusBar.h>
#include <ConfigEditorPopup.h>
#include <memory>

class BackgroundScannerDiscoveryEVH;
class ScanProjectHandler; // An event handler extending InsaneWorkerEvent.

// Page index (not page number), {pixel file path, {pixel count, image width, image height}}.
typedef std::map<uint, std::tuple<std::string, InsaneWorkerEvent::ImageAttributes>> PixelFilesMap;

/**
 * Shows a label, a disabled text box and a button.
 * 
 * Label:
 *  - Right click: define the number of pages to scan and double-sided scanning.
 *     - Number of pages: it's the number of sides, not the number of sheets.
 *     - Backface: if an automatic document feeder is used. If 5 pages are to be
 *       scanned, feed 3 pages on front face, turn the whole stack including the
 *       6th page to scan the backfaces; the last page will be discarded.
 *       With a feeder, the 'double-sided' option can be off if all backfaces
 *       are blank.
 *       With a flatbed scanner, the 'double-sided' should typically be off and
 *       the pages scanned in their logical order.
 * 
 * Text box:
 *  - shows the path and the basename of the files to create. This must be set
 *    by the application in any conveniant manner.
 *    Pressing the backspace key clears the text box.
 * 
 * Button:
 *  - Right click: shows the scanner widget.
 *  - Left click: starts scanning.
 */
class XInsaneWidget : public InsaneWidget
{
public:
  virtual ~XInsaneWidget();
  XInsaneWidget( wxWindow* parent, TimeredStatusBar * sb,  wxConfig * config, wxWindowID id = SYMBOL_INSANEWIDGET_IDNAME, const wxPoint& pos = SYMBOL_INSANEWIDGET_POSITION, const wxSize& size = SYMBOL_INSANEWIDGET_SIZE, long style = SYMBOL_INSANEWIDGET_STYLE );

  void ResetScanProject();
  void CancelScanning(); // Not tested, probably doesn't work as intended.
  void EnableScanButton(bool enable); // For CallAfter.
private:
  wxConfig * m_config;
  wxWeakRef<TimeredStatusBar> m_sb;

  // Contains a popup to define the number of pages and double-sided scanning.
  std::unique_ptr<ConfigEditorPopup> m_pageStack;
  // Contains the scanner widget.
  std::unique_ptr<wxPopupTransientWindow> m_ptwScannerWidget;
  // Available devices and minimal options.
  std::unique_ptr<XScannerWidget> m_scannerWidget;
  std::unique_ptr<InsaneWorker> m_insaneWorker;
  std::unique_ptr<BackgroundScannerDiscoveryEVH> m_backgroundScannerDiscoveryEvh;
  std::unique_ptr<ScanProjectHandler> m_scanProject;
  
  void OnLblNewDocRightClick ( wxMouseEvent& evt );
  void OnTxtNewDocKeyPressed ( wxKeyEvent& evt );
  void OnBtnScanRightClick ( wxMouseEvent& evt );
  void OnBtnScanClick ( wxMouseEvent& evt );
};

#endif // XINSANEWIDGET_H
