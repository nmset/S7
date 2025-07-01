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
    MiscTools::MessageBox(_("A scan library error occurred."), true);
  }
  void OnError ( const std::string& message ) override
  {
    cerr << message << endl;
    Reset();
    MiscTools::MessageBox(_("A general error occurred."), true);
  }
  void OnSessionReadError(const std::string & filePath) override
  {
    const wxString msg = _("A session read error occurred.");
    cerr << msg << endl;
    Reset();
    MiscTools::MessageBox(msg, true);
  }
  void OnSessionCancelled(const std::string & filePath) override
  {
    const wxString msg = _("Session cancelled.");
    Reset();
    MiscTools::MessageBox(msg, true);
  }
  // Every time a page is fully scanned.
  void OnPageEndScan(const std::string & filePath, uint pageIndex,
                     const ImageAttributes& imageAttributes) override
  {
    m_startPageIndex = pageIndex;
    m_pixelFiles[pageIndex] = {filePath, imageAttributes};
    
    auto informProgress = [&] ()
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
      
      m_sb->SetStatusText(msg);
    };

    if (m_outputType != PDF)
    {
      // Convert pixel file to PNG using netpbm.
      if (!PixelToImageWriter::Convert(filePath, imageAttributes.width, imageAttributes.height,
                                  m_stampDescriptors, m_outputType))
      {
        const wxString msg = _("Failed to create output image.");
        cerr << msg << endl;
        if (m_sb)
          m_sb->SetTransientText(msg);
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
          m_sb->SetTransientText(msg);
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
      m_owner->txtNewDoc->Clear();
      if (m_sb)
      {
        const wxString msg = _("Finished.");
        m_sb->SetTransientText(msg);
      }
    }
  }
  void OnStartScanSession(uint pageIndex, const ImageAttributes& imageAttributes) override
  {
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
  
};

// ----------------------------------------------------------------------------
IMPLEMENT_CLASS( XInsaneWidget, InsaneWidget )

XInsaneWidget::XInsaneWidget(wxWindow* parent, TimeredStatusBar * sb, wxConfig * config, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: InsaneWidget(parent, id, pos, size, style)
{
  UpdateExtensionsMap();
  m_config = config;
  m_sb = sb;
}

XInsaneWidget::~XInsaneWidget() = default; // Important for mixing unique_ptr and PIMPL.

void XInsaneWidget::Setup()
{
  lblNewDoc->Bind ( wxEVT_RIGHT_UP, &XInsaneWidget::OnLblNewDocRightClick, this );
  txtNewDoc->Bind ( wxEVT_KEY_UP, &XInsaneWidget::OnTxtNewDocKeyPressed, this );
  
  m_ptwScannerWidget = std::unique_ptr<wxPopupTransientWindow> (new wxPopupTransientWindow ( wxApp::GetGUIInstance()->GetTopWindow() ));
  m_ptwScannerWidget->Show ( false );
  m_scanProject = std::make_unique<ScanProjectHandler>(this, m_sb);
  m_insaneWorker = std::make_unique<InsaneWorker>(m_scanProject.get());
  m_scannerWidget = std::make_unique<XScannerWidget> ( m_ptwScannerWidget.get(), m_sb, m_insaneWorker.get() );
  m_scannerWidget->SetConfig ( m_config );

  btnScan->Enable(false);
  btnScan->Bind ( wxEVT_RIGHT_UP, &XInsaneWidget::OnBtnScanRightClick, this );
  btnScan->Bind ( wxEVT_LEFT_UP, &XInsaneWidget::OnBtnScanClick, this );
  m_backgroundScannerDiscoveryEvh = std::make_unique<BackgroundScannerDiscoveryEVH>(this);
  BackgroundScannerDiscovery * backgroundDiscovery = new BackgroundScannerDiscovery ( m_scannerWidget.get(),
                                                                                      m_backgroundScannerDiscoveryEvh.get());
  backgroundDiscovery->Run();
}


// Show a popup to specifiy the number of pages to scan and back-sided scanning.
void XInsaneWidget::OnLblNewDocRightClick ( wxMouseEvent& evt )
{
  /*
   * The previous ConfigEditorPopup is deleted here, which commits the
   * parameters to the config file. At any time, the current parameter values
   * can be lost if a crash occurs.
   */
  m_pageStack.reset(new ConfigEditorPopup(wxApp::GetGUIInstance()->GetTopWindow(), m_config));
  PopupTransientWindow * ptw = m_pageStack->CreatePopup();
  if ( !ptw )
  {
    evt.Skip();
    return;
  }
  wxCheckBox * cb = m_pageStack->AddCheckBox (_("Double sided:"),_T("/Scanner/DoubleSided") );
  cb->SetToolTip (_("Scan all front faces first, then all back faces in reverse order.") );
  wxSpinCtrl * spn = m_pageStack->AddSpinCtrl (_("Total:"),_T("/Scanner/Total") );
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
  const wxString dest = txtNewDoc->GetValue();
  if (dest.IsEmpty())
  {
    MiscTools::MessageBox(_("Destination file missing."), true);
    evt.Skip();
    return;
  }
  const uint outputType = m_scannerWidget->GetScannerOutputType();
  bool adf = false;
  bool doubleSided = false;
  uint total = 1;
  wxString paperSize = _T("A4");
  if (m_config)
  {
    doubleSided = m_config->ReadBool("/Scanner/DoubleSided", false);
    total = m_config->ReadLong("/Scanner/Total", 1);
    m_config->Read("/Scanner/Last/PaperSize", &paperSize, "A4");
  }

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
    m_scanProject->SetDoubleSided(doubleSided);
    m_scanProject->SetTotalNumberOfSides(total);
    m_stampDescriptors = m_stampWidgets->GetStampDescriptors();
    m_scanProject->SetStampDescriptors(m_stampDescriptors);
    
    std::pair<int, int> startAndIncrement = m_scanProject->GetStartAndIncrement(m_insaneWorker.get());
    
    const int padWidth = ( ushort ) m_config->Read (_T("/Scanner/Counter/Length"), 2 );
    bool res = m_insaneWorker->Scan(destFile.GetPath().ToStdString(),
                                    destFile.GetName().ToStdString(),
                                    startAndIncrement.first, padWidth, startAndIncrement.second);
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
  if (m_insaneWorker)
    m_insaneWorker->Cancel();
}

void XInsaneWidget::EnableScanButton(bool enable)
{
  btnScan->Enable(enable);
}
