/*
* File:   PixelToPdfWriter.h
* Author: Saleem Edah-Tally - nmset@yandex.com
* License : CeCILL-C
* Copyright Saleem Edah-Tally - © 2025
*
* Created on 14 06 2025, 17:15
*/

#ifndef PIXELTOPDFWRITER_H
#define PIXELTOPDFWRITER_H

#include <string>
#include <map>
#include <vector>
#include <podofo/podofo.h>
#include <wx/wx.h>

struct StampDescriptor;

/**
 * Create a PDF document, append or insert pages from raw scanned files.\n
 * Each image is a full page scan; it is scaled in the PDF document to the full 
 * page dimensions.\n
 * Account for page size.\n
 * Optionally, a stamp image may be blended on the page.
 */
class PixelToPdfWriter
{
public:
  typedef std::map<std::string, PoDoFo::PdfPageSize> PageSizeMap;

  PixelToPdfWriter();
  bool AddPageAt(const std::string& pixelFile, uint width, uint height, uint index,
          std::vector<StampDescriptor*> * descriptors,
          PoDoFo::PdfPageSize pageSize = PoDoFo::PdfPageSize::A4,
          PoDoFo::PdfColorSpace = PoDoFo::PdfColorSpace::DeviceRGB /*Unused*/);
  void Save(const std::string& pdfFile);
  uint GetNumberOfPages() const;
  void RemovePageAt(uint index);
  PoDoFo::PdfPageSize GetPageSize(const std::string& literal)
  {
    return m_pageSizes[literal]; // 0 if not found, PdfPageSize::Unknown.
  }
  PageSizeMap GetPageSizes() const
  {
    return m_pageSizes;
  }
private:
  PoDoFo::PdfMemDocument m_doc;

  PageSizeMap m_pageSizes;
};

#endif // PIXELTOPDFWRITER_H
