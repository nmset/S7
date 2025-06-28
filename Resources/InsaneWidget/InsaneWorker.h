/*
* File:   InsaneWorker.h
* Author: Saleem Edah-Tally - nmset@yandex.com
* License : CeCILL-C
* Copyright Saleem Edah-Tally - © 2024
*
* Created on 14 02 2024, 13:00
*/

#ifndef INSANEWORKER_H
#define INSANEWORKER_H

#include <libinsane/capi.h>
#include <string>
#include <vector>

class InsaneWorkerEvent;

/**
 * Performs scanning to raw data as files.\n
 * Input values are as obtained from the scanner widget:
 * 
 *  - device identifier - ex: sane:brother5:net1;dev0
 *  - source - ex: Automatic Document Feeder(centrally aligned)
 *  - source index - important for an ADF source
 *  - mode - Color, LineArt, anything else is Gray
 *  - resolution
 *  - paper size - only those handled by PoDoFo
 * 
 * Output files are numbered with a padded suffix according to an increment that
 * can be negative.
 */

/*
 * All C objects must be initialised.
 * Initialise to nullptr and not to NULL.
 * Otherwise, unhealthy things are observed,
 * like 'if (err)' always evaluated to true.
 */

class InsaneWorker
{
public:

  InsaneWorker ( InsaneWorkerEvent* evh = nullptr );
  ~InsaneWorker();
  struct DeviceDescriptor
  {
    std::string id;
    std::string model;
    std::string type;
    std::string vendor;
    std::string GetLabel()
    {
      return model + "-" + type + " [" + vendor + "]";
    }
  };

  bool Init(); // Must be explicitly called, once only.
  std::vector<DeviceDescriptor> GetDeviceDescriptors()
  {
    return m_devices;
  }
  int IsADF(const std::string& deviceId, int sourceChildIndex);;
  std::string GetOptionPossibleValues(const std::string& deviceId,
                              const std::string& optionName, std::vector<std::string>& possible);
  bool ConfigureDevice(const std::string& deviceId,
                       const std::string& source = "FlatBed", int sourceChildIndex = 0,
                       const std::string& mode = "Color", int resolution = 300,
                       const std::string& paperSize = "A4");
  bool Scan(const std::string& dir, const std::string& basename,
          int startIndex = 0, int padwidth = 2, int increment = 1); // increment can be negative
  void Cancel();
  InsaneWorkerEvent * GetEventHandler() const
  {
    return m_evh;
  }
  /**
   * Update the start page index and increment to pass to ::Scan(), accounting
   * for ADF and double sided scanning.
   */
  std::pair<int, int> UpdateStartAndIncrement(const int startPageIndex, const int increment,
                                              const bool adf, const bool doubleSided, const int total);

private:
  lis_api * m_api = nullptr;
  lis_item * m_rootSourceItem = nullptr; // May have child items.
  lis_item * m_sourceItem = nullptr;
  InsaneWorkerEvent * m_evh = nullptr;

  std::vector<DeviceDescriptor> m_devices;
  std::string m_deviceId;
  std::string m_source = "FlatBed";
  int m_sourceChildIndex = 0;
  std::string m_mode = "Color";
  int m_resolution = 300;
  std::string m_paperSize = "A4";

  bool GetBottomRight(std::pair<double, double>& br);
  std::string ToLower(const std::string& input);
};

class InsaneWorkerEvent
{
public:
  InsaneWorkerEvent();
  virtual ~InsaneWorkerEvent();

  struct ImageAttributes
  {
    size_t size = 0; // bytes
    uint width = 0; // pixels
    uint height = 0; // pixels
  };

  virtual void OnInsaneError ( const std::string& message );
  virtual void OnError ( const std::string& message );
  virtual void OnSessionCancelled(const std::string& filePath);
  virtual void OnSessionReadError(const std::string& filePath);
  virtual void OnPageStartScan(const std::string& filePath, uint pageIndex,
                               const ImageAttributes& imageAttributes);
  virtual void OnPageEndScan(const std::string& filePath, uint pageIndex,
                             const ImageAttributes& imageAttributes);
  virtual void OnStartScanSession(uint pageIndex, const ImageAttributes& imageAttributes);
  virtual void OnEndScanSession(uint pageIndex, const ImageAttributes& imageAttributes);
};

#endif // INSANEWORKER_H
