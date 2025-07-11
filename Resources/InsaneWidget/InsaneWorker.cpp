/*
* File:   InsaneWorker.cpp
* Author: Saleem Edah-Tally - nmset@yandex.com
* License : CeCILL-C
* Copyright Saleem Edah-Tally - © 2024
*
* Created on 14 02 2024, 13:00
*/

#include "InsaneWorker.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <paper.h>
#include <libinsane/safebet.h>
#include <libinsane/error.h>
#include <libinsane/util.h>
#include <libinsane/normalizers.h>
#include <libinsane/constants.h>

using namespace std;

#define IERR(e) ("[" + to_string(e) + "] " + lis_strerror(e))

InsaneWorker::InsaneWorker ( InsaneWorkerEvent * evh )
{
  m_evh = evh;
}

InsaneWorker::~InsaneWorker()
{
  if (m_api)
    m_api->cleanup(m_api);
}

bool InsaneWorker::Init()
{
  // Terminate a previous one.
  if (m_api)
  {
    m_devices.clear();
    m_deviceId.clear();
    m_api->cleanup(m_api);
    m_sourceItem = nullptr;
    m_rootSourceItem = nullptr;
  }
  lis_error err;
  err = lis_safebet ( &m_api );
  if ( err )
  {
    if ( m_evh )
    {
      m_evh->OnInsaneError ( IERR(err) );
      return false;
    }
  }

  lis_device_descriptor ** devices = nullptr; // nullptr terminated.
  err = m_api->list_devices ( m_api, LIS_DEVICE_LOCATIONS_LOCAL_ONLY, &devices ); // Gets network scanners though.
  if ( err )
  {
    m_evh->OnInsaneError ( IERR(err) );
    return false;
  }

  uint numberOfDevices = 0;
  for ( uint i = 0; devices[i] != nullptr; i++ )
  {
    numberOfDevices++;
  }
  // Collect available descriptions for all devices.
  for ( uint i = 0; i < numberOfDevices; i++ )
  {
    DeviceDescriptor dd;
    lis_device_descriptor * descriptor = devices[i];
    dd.id = descriptor->dev_id;
    dd.model = descriptor->model;
    dd.type = descriptor->type;
    dd.vendor = descriptor->vendor;
    m_devices.push_back(dd);
  }

  return true;
}

// Determine if a source item is a feeder.
int InsaneWorker::IsADF(const std::string& deviceId, int sourceChildIndex)
{
  if ( !m_api )
  {
    if ( m_evh )
    {
      m_evh->OnError ( "Insane api is NULL; has Init() been called?");
      return -1;
    }
  }
  lis_error err;
  lis_item * rootSourceItem = nullptr;
  err = m_api->get_device ( m_api, deviceId.c_str(), &rootSourceItem );
  if ( err )
  {
    if ( m_evh )
      m_evh->OnInsaneError ( IERR ( err ) );
    return false;
  }
  lis_item ** children = nullptr;
  err = rootSourceItem->get_children(rootSourceItem, &children);
  if ( err )
  {
    if ( m_evh )
      m_evh->OnInsaneError ( IERR ( err ) );
    return -1;
  }

  bool adf = (children[sourceChildIndex]->type == LIS_ITEM_ADF);
  rootSourceItem->close(rootSourceItem);
  return adf;
}

// Ex: all supported resolutions.
std::string InsaneWorker::GetOptionPossibleValues(const std::string& deviceId,
                                          const std::string& optionName, std::vector<std::string>& possible)
{
  if ( !m_api )
  {
    if ( m_evh )
    {
      m_evh->OnError ( "Insane api is NULL; has Init() been called?");
      return "";
    }
  }
  lis_error err;
  lis_item * item = nullptr;
  err = m_api->get_device ( m_api, deviceId.c_str(), &item );
  if ( err )
  {
    if ( m_evh )
      m_evh->OnInsaneError ( IERR ( err ) );
    return "";
  }

  lis_option_descriptor ** options = nullptr;
  err = item->get_options ( item, &options );
  if ( err )
  {
    if ( m_evh )
      m_evh->OnInsaneError ( IERR ( err ) );
    item->close(item);
    return "";
  }
  uint numberOfOptions = 0;
  for ( uint i = 0; options[i] != nullptr; i++ )
  {
    numberOfOptions++;
  }

  std::string defaultValue;
  for ( uint i = 0; i < numberOfOptions; i++ )
  {
    lis_option_descriptor * optionDescriptor = options[i];
    const std::string name = optionDescriptor->name;
    if ( ToLower(name) != optionName )
      continue;
    union lis_value value;
    err = optionDescriptor->fn.get_value ( optionDescriptor, &value );
    if ( err )
    {
      if ( m_evh )
        m_evh->OnInsaneError ( IERR ( err ) );
      item->close(item);
      return "";
    }
    lis_value_type type = optionDescriptor->value.type;
    bool isString = false;
    switch ( type )
    {
    case LIS_TYPE_BOOL:
      defaultValue = std::to_string(( bool ) value.boolean);
      break;
    case LIS_TYPE_INTEGER:
      defaultValue = std::to_string(( long ) value.integer);
      break;
    case LIS_TYPE_DOUBLE:
      defaultValue = std::to_string(( double ) value.dbl);
      break;
    case LIS_TYPE_STRING:
      isString = true;
      defaultValue = value.string;
      break;
    case LIS_TYPE_IMAGE_FORMAT:
      defaultValue = "Unmanaged";
      break;
    default:
      defaultValue = "Unknown";
      break;
    }
    lis_value_list valueList = optionDescriptor->constraint.possible.list;
    int numberOfListItems = valueList.nb_values;
    if ( numberOfListItems )
    {
      for ( int j = 0; j < numberOfListItems; j++ )
      {
        if ( isString )
          possible.push_back( valueList.values[j].string );
        else
          possible.push_back( std::to_string ( valueList.values[j].integer ) );
      }
    }
  }

  item->close(item);
  return defaultValue;
}

// Open a source item and prepare for the next step, i.e, scanning.
bool InsaneWorker::ConfigureDevice(const std::string& deviceId,
                                   const std::string& source, int sourceChildIndex,
                                   const std::string& mode, int resolution,
                                   const std::string& paperSize)
{
  if ( !m_api )
  {
    if ( m_evh )
    {
      m_evh->OnError ( "Insane api is NULL; has Init() been called?");
      return false;
    }
  }
  m_deviceId = deviceId;
  m_source = source;
  m_sourceChildIndex = (sourceChildIndex > -1) ? sourceChildIndex : 0;
  m_mode = mode;
  m_resolution = resolution;
  m_paperSize = paperSize;

  lis_error err;

  err = m_api->get_device ( m_api, m_deviceId.c_str(), &m_rootSourceItem );
  if ( err )
  {
    if ( m_evh )
      m_evh->OnInsaneError ( IERR ( err ) );
    return false;
  }

  lis_item ** children = nullptr;
  err = m_rootSourceItem->get_children(m_rootSourceItem, &children);
  if ( err )
  {
    if ( m_evh )
      m_evh->OnInsaneError ( IERR ( err ) );
    return false;
  }
  
  // Selecting a lis_item by the source index.
  // One child item is created by libinsane if there is no source: v4l.
  m_sourceItem = children[m_sourceChildIndex];
  if (!m_sourceItem)
  {
    if (m_evh)
      m_evh->OnError("Could not determine a device item");
    m_rootSourceItem->close(m_rootSourceItem);
    return false;
  }

  // Error status not checked; use defaults if failure.
  lis_set_option(m_sourceItem, OPT_NAME_MODE, mode.c_str());
  /*
   * Selecting a source on a child item that has been selected by a source index.
   * Sounds weird but needed.
   */
  lis_set_option(m_sourceItem, OPT_NAME_SOURCE, source.c_str());
  if (resolution > 0) // No resolution with v4l devices.
    lis_set_option(m_sourceItem, OPT_NAME_RESOLUTION, to_string(resolution).c_str());
  pair<double, double> br;
  if (GetBottomRight(br))
  {
    // For v4l, these do not get applied.
    lis_set_option(m_sourceItem, "br-x", to_string(br.first).c_str());
    lis_set_option(m_sourceItem, "br-y", to_string(br.second).c_str());
  }
  
  // Don't close m_rootDeviceItem, it is prepared for ::Scan().
  return true;
}

// Perform scanning: create raw data files.
bool InsaneWorker::Scan(const std::string& dir, const std::string& basename,
                        int startIndex, int padwidth, int increment)
{
  if ( !m_api || !m_sourceItem )
  {
    if ( m_evh )
    {
      m_evh->OnError ( "Insane api or device item is NULL, cannot scan.");
      return false;
    }
  }
  if (dir.empty() || basename.empty() || startIndex < 0 || padwidth < 0 || (increment == 0))
  {
    if ( m_evh )
    {
      m_evh->OnError ( "Invalid input, cannot scan.");
      return false;
    }
  }
  if (m_cancelRequested)
    return false;
  auto makeFileName = [&] (int index)
  {
    // std:format is not friendly with a variable padwidth; requires a literal format.
    stringstream ss;
    ss << setfill('0') << setw(padwidth) << index;
    return (dir + "/" + basename + "-" + ss.str());
  };

  lis_error err;
  
  lis_scan_session * session;
  // Scanning starts here on device.
  err = m_sourceItem->scan_start(m_sourceItem, &session);
  if (err || !session)
  {
    if ( m_evh )
      m_evh->OnInsaneError ( IERR ( err ) );
    return false;
  }

  lis_scan_parameters parameters; // DO NOT use a POINTER here.
  err = session->get_scan_parameters(session, &parameters);
  if (err)
  {
    if ( m_evh )
      m_evh->OnInsaneError ( IERR ( err ) );
    return false;
  }

  /*
   * format is troublesome: always 0.
   * But for LineArt, in the logs, sane_get_parameters() returns format=1 (v4l)
   * or format=2 (remote).
   */
  // lis_img_format format = parameters.format; // guaranteed, but always 0, even for LineArt.
  const int lineArtFactor = (m_mode == string("LineArt") ? 3 : 1); // insane always outputs RGB.
  int imageWidth = parameters.width; // pixels, guaranteed.
  int imageHeight = parameters.height; // pixels, not guaranteed.
  size_t imageSize = parameters.image_size * lineArtFactor; // bytes, not guaranteed.
  InsaneWorkerEvent::ImageAttributes imageAttributes;
  imageAttributes.size = imageSize;
  imageAttributes.width = imageWidth;
  imageAttributes.height = imageHeight;

  // This file is valid raw pixels. It can be converted with rawtoppm and rawtopgm.
  uint pageIndex = startIndex;
  string filePath = makeFileName(pageIndex);
  const int bytesPerRow = imageSize / imageHeight;
  /*
   * Needs confirmation.
   * LineArt : 1 pixel = 1 bit - maximum resolution = 9600
   * Gray : 1 pixel = 1 byte - maximum resolution = 2400
   * Color : 1 pixel = 3 bytes - maximum resolution = 600
   * 
   * A4, LineArt, 4800: 1690000620 bytes file OK (1.6 GiB).
   * Above that resolution:
   * std::bad_alloc - 1993034797 bytes (1.856 GiB) cannot be allocated
   * for one row. System with 64 GB RAM.
   */

  if (m_evh)
    m_evh->OnStartScanSession(startIndex, imageAttributes);
  do
  {
    ofstream outFile;
    if (m_evh)
      m_evh->OnPageStartScan(filePath, pageIndex, imageAttributes);
    do
    {
      if (m_cancelRequested)
      {
        session->cancel(session);
        m_rootSourceItem->close(m_rootSourceItem);
        if (m_evh)
          m_evh->OnSessionCancelled(filePath);
        m_cancelRequested = false;
        return false;
      }
      try
      {
        char * readBytes = new char[bytesPerRow];
        size_t bufferSize = bytesPerRow;
        err = session->scan_read(session, readBytes, &bufferSize); // bufferSize is in/out.
        if (LIS_IS_ERROR(err))
        {
          delete[] readBytes;
          if (m_evh)
            m_evh->OnSessionReadError(filePath);
          return false;
        }
        /*
        * bufferSize == 0 does not mean end_of_page or end of file.
        * But we get here when end_of_feed is false while there's nothing more to read.
        */
        if (bufferSize)
        {
          if (!outFile.is_open())
            outFile.open(filePath, ios::binary | ios::trunc);
          outFile.write((const char*) readBytes, bufferSize);
        }
        delete[] readBytes;
        }
      catch (std::bad_alloc& e)
      {
        m_rootSourceItem->close(m_rootSourceItem);
        cerr << "ABORT: " << e.what() << " - could not allocate " << bytesPerRow << " bytes." << endl;
        if (m_evh)
          m_evh->OnError("Insufficient system RAM.");
        return false;
      }
    } while (!session->end_of_page(session));
    /*
     * end_of_feed may return false though there is no more to read.
     * If outFile is not open, nothing was read; don't call OnPageEndScan.
     */
    bool outFileWasOpen = outFile.is_open();
    // It may not have been opened.
    outFile.flush();
    outFile.close();
    if (m_evh && outFileWasOpen)
      m_evh->OnPageEndScan(filePath, pageIndex, imageAttributes);
    pageIndex += increment;
    if (m_sourceItem->type != LIS_ITEM_ADF) // v4l also
      break;
    filePath = makeFileName(pageIndex);
  } while( !session->end_of_feed(session));
  if (m_evh)
    m_evh->OnEndScanSession(pageIndex - increment, imageAttributes);

  /*
   * Don't close the child item.
   * A v4l device may become unusable after the first capture (its child source
   * item may have been faked by libinsane if it had none).
   * Real scanners sustain both situations so far.
   */
  m_rootSourceItem->close(m_rootSourceItem); // Root.
  return true;
}

// Determine the page extent.
bool InsaneWorker::GetBottomRight(std::pair<double, double>& br)
{
  int res = paperinit();
  if (res != PAPER_OK)
  {
    const string msg = "Could not initialise the paper library.";
    cerr << msg << endl;
    if (m_evh)
      m_evh->OnError(msg);
    return false;
  }
  
  const paper * p = paperinfo(m_paperSize.c_str());
  if (!p)
  {
    string msg = "Failed to find the requested paper; attempt to use a default paper size.";
    cerr << msg << endl;
    if (m_evh)
      m_evh->OnError(msg);
    p = defaultpaper();
    if (!p)
    {
      msg = "Failed to find a default paper size; using the default scanner sizes.";
      cerr << msg << endl;
      if (m_evh)
        m_evh->OnError(msg);
      return false;
    }
  }

  int unit = paperunit(p);
  double conversionFactor = 1.0;
  if (unit == PAPER_UNIT_MM)
    conversionFactor = 1.0;
  else if (unit == PAPER_UNIT_IN)
    conversionFactor = 25.4;
  else
  {
    const string msg = "The measurement unit of the paper size is not handled; using the default scanner sizes.";
    cerr << msg << endl;
    if (m_evh)
      m_evh->OnError(msg);
    return false;
  }

  br.first = paperwidth(p) * conversionFactor;
  br.second = paperheight(p) * conversionFactor;

  res = paperdone();
  if (res != PAPER_OK)
  {
    const string msg = "Could not cleanly end the paper library.";
    cerr << msg << endl;
    if (m_evh)
      m_evh->OnError(msg);
  }
  return true;
}


std::string InsaneWorker::ToLower(const std::string& input)
{
  // We are dealing with single byte characters.
  string output;
  for ( char c : input )
    output.append ( 1, std::tolower ( c ) );
  return output;
}

void InsaneWorker::Cancel()
{
  m_cancelRequested = true;
}

std::pair<int, int> InsaneWorker::UpdateStartAndIncrement(const int startPageIndex, const int increment,
                                                          const bool adf, const bool doubleSided, const int total)
{
  int newStartPageIndex = startPageIndex;
  int newIncrement = increment;
  int totalEven = total;
  if (total % 2)
    totalEven++;
  // Scanning forward before max.
  if ((increment > 0) && adf && doubleSided && (startPageIndex < (totalEven - 2)))
  {
    newIncrement = 2;
    newStartPageIndex += newIncrement;
  }
  // Scanning forward at max.
  else if ((increment > 0) && adf && doubleSided && (startPageIndex == (totalEven - 2)))
  {
    newIncrement = -2;
    newStartPageIndex++;
  }
  // Scanning backward.
  else if ((increment < 0) && adf && doubleSided && (startPageIndex <= (totalEven - 1)))
  {
    newIncrement = -2;
    newStartPageIndex += newIncrement;
  }
  else
  {
    newIncrement = 1;
    newStartPageIndex++;
  }
  
  std::pair<int, int> result = {newStartPageIndex, newIncrement};
  return result;
}

// ----------------------------------------------------------------------------
InsaneWorkerEvent::InsaneWorkerEvent()
{}

InsaneWorkerEvent::~InsaneWorkerEvent()
{}

void InsaneWorkerEvent::OnInsaneError ( const std::string& message )
{}

void InsaneWorkerEvent::OnError ( const std::string& message )
{}

void InsaneWorkerEvent::OnSessionCancelled(const std::string& filePath)
{}

void InsaneWorkerEvent::OnSessionReadError(const std::string& filePath)
{}

void InsaneWorkerEvent::OnPageStartScan(const std::string& filePath,uint pageIndex, const ImageAttributes& imageAttributes)
{}

void InsaneWorkerEvent::OnPageEndScan(const std::string& filePath, uint pageIndex, const ImageAttributes& imageAttributes)
{}

void InsaneWorkerEvent::OnStartScanSession(uint pageIndex, const ImageAttributes& imageAttributes)
{}

void InsaneWorkerEvent::OnEndScanSession(uint pageIndex, const ImageAttributes& imageAttributes)
{}
