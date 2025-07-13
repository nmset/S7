/*
* File:   XInsaneWidget.cpp
* Author: Saleem Edah-Tally - nmset@yandex.com
* License : CeCILL-C
* Copyright Saleem Edah-Tally - © 2025
*
* Created on 15 06 2025, 21:24
*/

#include "XInsaneWidget.h"
#include <PopupTransientWindow.h>
#include <MiscTools.h>
#include "PixelToImageWriter.h"
#include "PixelToPdfWriter.h"
#include <DefsInsaneWidget.h>
#include <DefsStampWidget.h>
#include <XStampWidget.h>
#include <StampWorker.h>

using namespace std;

// When device discovery in a detached thread completes.
// ----------------------------------------------------------------------------
class BackgroundScannerDiscoveryEVH : public BackgroundScannerDiscoveryEvent
{
public:
  BackgroundScannerDiscoveryEVH(XInsaneWidget * owner)
  {
    m_owner = owner;
  }
  virtual ~BackgroundScannerDiscoveryEVH()
  {}
  void OnDone() override
  {
    if (!m_owner)
      return;
    m_owner->CallAfter(&XInsaneWidget::EnableScanButton, true);
  }
private:
  wxWeakRef<XInsaneWidget> m_owner = nullptr;
};

// ----------------------------------------------------------------------------
class BackgroundScan : public wxThread
{
public:
  BackgroundScan(XInsaneWidget * insaneWidget, InsaneWorker * insaneWorker,
                 const wxString& dir, const wxString& basename,
                 int start = 0, int padWidth = 2, int increment = 1)
  {
    m_insaneWidget = insaneWidget;
    m_insaneWorker = insaneWorker;
    m_dir = dir;
    m_basename = basename;
    m_start = start;
    m_padWidth = padWidth;
    m_increment = increment;
  }
  ExitCode Entry() override
  {
    /*
     * We refrain from using a mutex/critical section.
     * If 2 instances of XInsaneWidget in a single application scan on the same
     * device simultaneously, the first one that starts scanning prevails and
     * the second one triggers a "Device busy" error.
     * If 2 instances of XInsaneWidget in a single application scan on different
     * devices simultaneously, both jobs succeed.
     * Since there cannot be concurrency leading to a crash ('Device busy'), we
     * do not block the next line. It may limit an application that needs to
     * scan from multiple devices.
     */
    m_insaneWorker->Scan(m_dir.ToStdString(), m_basename.ToStdString(), m_start, m_padWidth, m_increment);
    return (ExitCode) 0;
  }
private:
  wxWeakRef<XInsaneWidget> m_insaneWidget = nullptr;
  InsaneWorker * m_insaneWorker = nullptr;
  wxString m_dir, m_basename;
  int m_start = 0, m_padWidth = 2, m_increment = 1;
};

// ----------------------------------------------------------------------------
class ScanProjectHandler : public InsaneWorkerEvent
{
public:
  ScanProjectHandler(XInsaneWidget * owner, TimeredStatusBar * sb = nullptr)
  : InsaneWorkerEvent(), m_owner(owner), m_sb (sb)
  {
  }
  /*
   * mode, resolution, outputType, adf, doubleSided, total, PaperSize:
   * These are not updated here if there are pending jobs.
   * However,
   *  deviceId, source, sourceIndex, mode, resolution, paperSize
   * can be changed if there are pending jobs in ConfigureDevice() since it is
   * called before these setters. They won't change on their own but by user
   * interaction.
   * The 'resolution' variable is used downstream to distinguish between v4l
   * and real scanners. v4l devices do not have a source and a resolution
   * tunables.
   */
  void SetMode(const string& mode)
  {
    if (m_pixelFiles.size())
      return;
    m_mode = mode;
  }
  void SetResolution(int resolution)
  {
    if (m_pixelFiles.size())
      return;
    m_resolution = resolution;
  }
  void SetOutputType(int outputType)
  {
    if (m_pixelFiles.size())
      return;
    m_outputType = outputType;
  }
  void SetADF(bool adf)
  {
    if (m_pixelFiles.size())
      return;
    m_adf = adf;
    if (m_adf && m_doubleSided)
      m_startPageIndex = -2;
    else
      m_startPageIndex = -1;
  }
  void SetDoubleSided(bool doubleSided)
  {
    if (m_pixelFiles.size())
      return;
    m_doubleSided = doubleSided;
    if (m_adf && m_doubleSided)
      m_startPageIndex = -2;
    else
      m_startPageIndex = -1;
  }
  void SetTotalNumberOfSides(uint total)
  {
    if (m_pixelFiles.size())
      return;
    m_total = total;
    m_totalEven = total;
    if (total % 2)
      m_totalEven = total + 1;
  }
  void SetPaperSize(const wxString& paperSize)
  {
    if (m_pixelFiles.size())
      return;
    m_paperSize = paperSize;
  }
  void SetStampDescriptors(vector<StampDescriptor*> * descriptors)
  {
    m_stampDescriptors = descriptors;
  }
  // WIP means InsaneWorker is busy, not the position in a scan project.
  void SetWip(bool wip)
  {
    m_wip = wip;
  }
  bool GetWip() const
  {
    return m_wip;
  }
  /* Enable/disable the label and destination file controls per scanning and
   * idle mode.
   * Do not include in Reset() because the status of the scan button is
   * independent of the rest.
   */
  void UpdateControlsStatus(bool enable)
  {
    wxTheApp->CallAfter([this, enable] ()
      {
        m_owner->lblNewDoc->Enable(enable);
        m_owner->txtNewDoc->Enable(enable);
        if (enable)
        {
          m_owner->btnScan->Enable(enable);
          m_owner->btnScan->SetLabelText(_("Scan"));
        }
        else
        {
          /*Only XInsaneWidget::Setup() and XInsaneWidget::CancelScanning()
           * disable the scan button.
           */
          m_owner->btnScan->SetLabelText(_("Cancel"));
        }
      }
    );
  }
  std::pair<int, int> GetStartAndIncrement(InsaneWorker * insaneWorker)
  {
    wxASSERT_MSG(insaneWorker != nullptr, "insaneWorker is NULL.");
    std::pair<int, int> startAndIncrement = insaneWorker->UpdateStartAndIncrement(m_startPageIndex, m_increment,
                                                                       m_adf, m_doubleSided, m_total);
    m_startPageIndex = startAndIncrement.first;
    m_increment = startAndIncrement.second;
    return startAndIncrement;
  }
  void OnInsaneError ( const std::string& message ) override
  {
    cerr << message << endl;
    Reset();
    UpdateControlsStatus(true);
    const wxString msg = _("A scan library error occurred.");
    MiscTools::AsyncMessageBox(msg, true);
  }
  void OnError ( const std::string& message ) override
  {
    cerr << message << endl;
    Reset();
    UpdateControlsStatus(true);
    const wxString msg = _("A general error occurred.");
    MiscTools::AsyncMessageBox(msg, true);
  }
  void OnSessionReadError(const std::string & filePath) override
  {
    const wxString msg = _("A session read error occurred.");
    cerr << msg << endl;
    Reset();
    UpdateControlsStatus(true);
    MiscTools::AsyncMessageBox(msg, true);
  }
  void OnSessionCancelled(const std::string & filePath) override
  {
    if (wxFileExists(filePath))
      wxRemoveFile(filePath);
    Reset();
    UpdateControlsStatus(true);
    const wxString msg = _("Session cancelled.");
    MiscTools::AsyncMessageBox(msg, true);
    m_sb->CallAfter(&TimeredStatusBar::SetTransientText, msg, 3000);
  }
  // Every time a page is fully scanned.
  void OnPageEndScan(const std::string & filePath, uint pageIndex,
                     const ImageAttributes& imageAttributes) override
  {
    m_startPageIndex = pageIndex;
    m_pixelFiles[pageIndex] = {filePath, imageAttributes};
    
    auto informProgress = [this] ()
    {
      if (!m_sb || (m_total == 1))
        return;
      int max = (m_adf && m_doubleSided) ? m_totalEven : m_total;
      wxString progress = to_string(m_pixelFiles.size()) + "/" + to_string(max);
      wxString info = _("Scanning: ");
      if (m_increment == 2)
        info = _("Front face: ");
      else if (m_increment == -2)
        info = _("Back face: ");
      wxString msg = info + progress;
      if (m_increment == 2 && ((max / 2) == (m_pixelFiles.size())))
      {
        wxString upperBoundMessage = _(". Turn the whole stack of pages.");
        msg += upperBoundMessage;
      }
      m_sb->CallAfter(&wxStatusBar::SetStatusText, msg, 0);
    };

    if (m_outputType != PDF)
    {
      // Convert pixel file to PNG using wxImage.
      if (!PixelToImageWriter::Convert(filePath, imageAttributes.width, imageAttributes.height,
                                  m_stampDescriptors, m_outputType))
      {
        const wxString msg = _("Failed to create output image.");
        cerr << msg << endl;
        if (m_sb)
          m_sb->CallAfter(&TimeredStatusBar::SetTransientText, msg, 3000);
      }
      wxRemoveFile(filePath);
      informProgress();
    }
    else
    {
      // Append or insert PDF page from pixel file using PoDoFo.
      if (!m_pixelToPdfWriter.get())
        m_pixelToPdfWriter = std::unique_ptr<PixelToPdfWriter> (new PixelToPdfWriter());
      uint index = (m_increment > 0) // 1 or 2
              ? m_pixelToPdfWriter->GetNumberOfPages() // Append.
              : m_totalEven - m_pixelToPdfWriter->GetNumberOfPages(); // Insert.
      PoDoFo::PdfPageSize pageSize = m_pixelToPdfWriter->GetPageSize(m_paperSize.ToStdString());
      if (pageSize == PoDoFo::PdfPageSize::Unknown)
      {
        const wxString msg = _("Wrong paper size: ") + m_paperSize + _("; using A4.");
        cerr << msg << endl;
        pageSize = PoDoFo::PdfPageSize::A4;
      }
      if (!m_pixelToPdfWriter->AddPageAt(filePath, imageAttributes.width, imageAttributes.height, index,
                              m_stampDescriptors, pageSize))
      {
        const wxString msg = _("Failed to add page to PDF document.");
        cerr << msg << endl;
        if (m_sb)
          m_sb->CallAfter(&TimeredStatusBar::SetTransientText, msg, 3000);
      }
      wxRemoveFile(filePath);
      informProgress();
    }
  }
  void OnEndScanSession(uint pageIndex, const ImageAttributes & imageAttributes) override
  {
    int max = (m_adf && m_doubleSided) ? m_totalEven : m_total;
    // A special case is made for (total == 1): double-sided does not have meaning.
    if (m_total == 1)
      max = m_total;
    if (m_pixelFiles.size() >= max)
    {
      // All pages have been scanned, accounting for ADF and double-sided.
      if (m_outputType != PDF)
      {
        // Remove the last image if an odd number of pages was requested.
        if (m_increment != 1 && (m_total != m_totalEven))
        {
          const string filePath = std::get<0> (m_pixelFiles.rbegin()->second) + "." + Extensions[m_outputType];
          wxRemoveFile(filePath);
        }
      }
      else
      {
        // Remove the last page if an odd number of pages was requested.
        if (m_increment != 1 && (m_total != m_totalEven))
        {
          uint lastIndex = m_pixelFiles.end()->first;
          m_pixelToPdfWriter->RemovePageAt(lastIndex - 1);
        }
        const string filePath = m_owner->txtNewDoc->GetValue().ToStdString() + "." + Extensions[m_outputType];
        m_pixelToPdfWriter->Save(filePath);
        m_pixelToPdfWriter.reset((new PixelToPdfWriter())); // For next scan project.
      }
      Reset();
      m_owner->txtNewDoc->CallAfter(&wxTextCtrl::SetValue, wxString());
      if (m_sb)
      {
        const wxString msg = _("Finished.");
        m_sb->CallAfter(&TimeredStatusBar::SetTransientText, msg, 3000);
      }
    }
    UpdateControlsStatus(true);
    m_wip = false;
  }
  void OnStartScanSession(uint pageIndex, const ImageAttributes& imageAttributes) override
  {
    // Dealing with stamps only.
    if (!m_stampDescriptors)
      return;
    for (StampDescriptor * descriptor : *m_stampDescriptors)
    {
      if (descriptor->text.IsEmpty())
        continue;
      descriptor->image = StampWorker::CreateStamp(descriptor, m_resolution);
    }
  }
  void Reset()
  {
    // Don't reset calculated variables that depend on widgets.
    m_startPageIndex = 0;
    m_increment = 1;
    m_totalEven = 0;
    m_pixelFiles.clear();
    m_wip = false;
  }
  
private:
  wxWeakRef<XInsaneWidget> m_owner = nullptr;
  wxWeakRef<TimeredStatusBar> m_sb = nullptr;
  std::unique_ptr<PixelToPdfWriter> m_pixelToPdfWriter;
  PixelFilesMap m_pixelFiles;
  vector<StampDescriptor*> * m_stampDescriptors = nullptr;
  string m_mode = "Color";
  int m_resolution = -1;
  uint m_outputType = PDF;
  wxString m_paperSize = _T("A4");
  bool m_adf = false;
  bool m_doubleSided = false;
  int m_total = 0;
  int m_totalEven = 0;
  int m_startPageIndex = 0;
  int m_increment = 1;
  bool m_wip = false; // Is InsaneWorker busy?
  
};

// ----------------------------------------------------------------------------
IMPLEMENT_CLASS( XInsaneWidget, InsaneWidget )

XInsaneWidget::XInsaneWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: InsaneWidget(parent, id, pos, size, style)
{}

XInsaneWidget::~XInsaneWidget() = default; // Important for mixing unique_ptr and PIMPL.

void XInsaneWidget::Setup(wxConfig * config, TimeredStatusBar * sb)
{
  UpdateExtensionsMap();
  m_config = config;
  m_sb = sb;

  // Restore known last values.
  m_doubleSided = m_config->ReadBool("/Scanner/DoubleSided", false);
  m_total = m_config->ReadLong("/Scanner/Total", 1);

  lblNewDoc->Bind ( wxEVT_RIGHT_UP, &XInsaneWidget::OnLblNewDocRightClick, this );
  txtNewDoc->Bind ( wxEVT_KEY_UP, &XInsaneWidget::OnTxtNewDocKeyPressed, this );
  
  /* 
   * Don't use GetTopWindow() as parent. If it goes away in an application with
   * multiples instances of XInsaneWidget, any call to the scanner widget leads
   * to a crash.
   */
  m_ptwScannerWidget = std::make_unique<wxPopupTransientWindow> (GetParent());
  m_ptwScannerWidget->Show ( false );
  m_scanProject = std::make_unique<ScanProjectHandler>(this, m_sb);
  m_insaneWorker = std::make_unique<InsaneWorker>(m_scanProject.get());
  m_scannerWidget = std::make_unique<XScannerWidget> (m_ptwScannerWidget.get());
  m_scannerWidget->Setup ( m_config, m_insaneWorker.get(), m_sb );

  btnScan->Enable(false);
  btnScan->Bind ( wxEVT_RIGHT_UP, &XInsaneWidget::OnBtnScanRightClick, this );
  btnScan->Bind ( wxEVT_LEFT_UP, &XInsaneWidget::OnBtnScanClick, this );
  m_backgroundDiscovery = new BackgroundScannerDiscovery ( m_scannerWidget.get(),
                                                          new BackgroundScannerDiscoveryEVH(this)); // Takes ownership.
  m_backgroundDiscovery->Run();
}


// Show a popup to specifiy the number of pages to scan and back-sided scanning.
void XInsaneWidget::OnLblNewDocRightClick ( wxMouseEvent& evt )
{
  if (!lblNewDoc->IsEnabled())
  {
    evt.Skip();
    return;
  }
  /*
   * The previous ConfigEditorPopup is deleted here, which commits the
   * parameters to the config file. At any time, the current parameter values
   * can be lost if a crash occurs.
   */
  m_pageStack.reset(new ConfigEditorPopup(GetParent(), m_config));
  PopupTransientWindow * ptw = m_pageStack->CreatePopup();
  if ( !ptw )
  {
    evt.Skip();
    return;
  }
  wxCheckBox * cb = m_pageStack->AddCheckBox (_("Double sided:"),_T("/Scanner/DoubleSided"), &m_doubleSided );
  cb->SetToolTip (_("Scan all front faces first, then all back faces in reverse order.") );
  wxSpinCtrl * spn = m_pageStack->AddSpinCtrl (_("Total:"),_T("/Scanner/Total"), &m_total );
  spn->SetRange ( 1, 50 );
  spn->SetToolTip (_("Total number of sides to scan (not total number of sheets).") );
  m_pageStack->ShowPopup();
  evt.Skip();
}

void XInsaneWidget::OnTxtNewDocKeyPressed ( wxKeyEvent& evt )
{
  if ( evt.GetKeyCode() == WXK_BACK )
  {
    txtNewDoc->Clear();
  }
  evt.Skip();
}

// Show the scanner widget or the stamp widgets.
void XInsaneWidget::OnBtnScanRightClick ( wxMouseEvent& evt )
{
  if (evt.ControlDown())
  {
    if (!m_dlgStampWidgets)
    {
      m_dlgStampWidgets = std::make_unique<wxDialog>
                            (nullptr, wxID_ANY, _("Stamps"),
                            wxDefaultPosition, wxDefaultSize,
                            wxCAPTION | wxRESIZE_BORDER | wxCLOSE_BOX);
      m_dlgStampWidgets->SetSize(600, 500);
      m_dlgStampWidgets->SetSizer(new wxBoxSizer(wxVERTICAL));
      m_dlgStampWidgets->Show ( false );
      m_stampWidgets = std::make_unique<XStampWidgets> ( m_dlgStampWidgets.get());
      m_stampWidgets->Setup(m_config);
      m_dlgStampWidgets->GetSizer()->Add(m_stampWidgets.get(), 1, wxGROW | wxALL, 5);
    }
    m_dlgStampWidgets->Show();
  }
  else
  {
    if ( m_scannerWidget->cmbDevices->GetCount() )
    {
      const wxSize current = m_scannerWidget->GetSize();
      m_scannerWidget->SetSize ( wxSize ( 500, current.GetHeight() ) );
    }
    MiscTools::ShowTransientPopup ( m_ptwScannerWidget.get(), m_scannerWidget.get() );
  }
  // +++ Prevent OnLblNewDocRightClick() from being called.
  // Why is OnBtnScanRightClick() not called the other way round?
  evt.Skip(false); 
}

// Start scanning.
void XInsaneWidget::OnBtnScanClick ( wxMouseEvent& evt )
{
  if (m_scanProject->GetWip())
  {
    CancelScanning();
    evt.Skip();
    return;
  }
  const wxString dest = txtNewDoc->GetValue();
  if (dest.IsEmpty())
  {
    MiscTools::MessageBox(_("Destination file missing."), true);
    evt.Skip();
    return;
  }
  const uint outputType = m_scannerWidget->GetScannerOutputType();
  bool adf = false;
  wxString paperSize = m_scannerWidget->GetPaperSize();

  wxFileName destFile(dest);
  const string deviceId = m_scannerWidget->GetCurrentDeviceId().ToStdString();
  const std::pair<int, wxString> sourceAttributes = m_scannerWidget->GetScannerSource();
  const string source = sourceAttributes.second.ToStdString();
  const string mode = m_scannerWidget->GetScannerMode().ToStdString();
  int resolution = -1; // No resolution with v4l devices.
  if ( !m_scannerWidget->GetScannerResolution().IsEmpty() )
  {
    resolution = std::stoi ( m_scannerWidget->GetScannerResolution().ToStdString() );
  }
  int sourceIndex = sourceAttributes.first == wxNOT_FOUND
                ? 0 : sourceAttributes.first;
  adf = m_insaneWorker->IsADF(deviceId, sourceIndex);
  if (m_insaneWorker->ConfigureDevice(deviceId, source, sourceIndex, mode, resolution, paperSize.ToStdString()))
  {
    m_scanProject->SetADF(adf);
    m_scanProject->SetMode(mode);
    m_scanProject->SetResolution(resolution);
    m_scanProject->SetOutputType(outputType);
    m_scanProject->SetPaperSize(paperSize);
    m_scanProject->SetDoubleSided(m_doubleSided);
    m_scanProject->SetTotalNumberOfSides((uint) m_total);
    if (m_stampWidgets)
    {
      m_stampDescriptors = m_stampWidgets->GetStampDescriptors();
      m_scanProject->SetStampDescriptors(m_stampDescriptors);
    }
    
    std::pair<int, int> startAndIncrement = m_scanProject->GetStartAndIncrement(m_insaneWorker.get());
    
    const int padWidth = ( ushort ) m_config->Read (_T("/Scanner/Counter/Length"), 2 );
    m_scanProject->UpdateControlsStatus(false);
    BackgroundScan * bgScan = new BackgroundScan(this, m_insaneWorker.get(),
                                                 destFile.GetPath().ToStdString(),
                                                 destFile.GetName().ToStdString(),
                                                 startAndIncrement.first, padWidth,
                                                 startAndIncrement.second);
    m_scanProject->SetWip(true);
    bgScan->Run();
  }
  evt.Skip();
}

void XInsaneWidget::ResetScanProject()
{
  if (m_scanProject)
    m_scanProject->Reset();
}

void XInsaneWidget::CancelScanning()
{
  if (m_insaneWorker && m_scanProject->GetWip())
  {
    m_insaneWorker->Cancel();
    btnScan->Enable(false);
  }
}

void XInsaneWidget::EnableScanButton(bool enable)
{
  // Called from BackgroundScannerDiscoveryEVH::Done() only;
  btnScan->Enable(enable);
  m_backgroundDiscovery = nullptr;
}

bool XInsaneWidget::IsScannerDiscoveryRunning()
{
  if (!m_backgroundDiscovery)
    return false;
  return m_backgroundDiscovery->IsRunning();
}

bool XInsaneWidget::IsScanning()
{
  if (!m_scanProject)
    return false;
  return m_scanProject->GetWip();
}
