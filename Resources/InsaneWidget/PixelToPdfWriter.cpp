/*
* File:   PixelToPdfWriter.cpp
* Author: Saleem Edah-Tally - nmset@yandex.com
* License : CeCILL-C
* Copyright Saleem Edah-Tally - © 2025
*
* Created on 14 06 2025, 17:15
*/

#include "PixelToPdfWriter.h"
#include <iostream>
#include <memory>

using namespace std;
using namespace PoDoFo;

PixelToPdfWriter::PixelToPdfWriter()
{
  m_pageSizes["A0"] = PoDoFo::PdfPageSize::A0;
  m_pageSizes["A1"] = PoDoFo::PdfPageSize::A1;
  m_pageSizes["A2"] = PoDoFo::PdfPageSize::A2;
  m_pageSizes["A3"] = PoDoFo::PdfPageSize::A3;
  m_pageSizes["A4"] = PoDoFo::PdfPageSize::A4;
  m_pageSizes["A5"] = PoDoFo::PdfPageSize::A5;
  m_pageSizes["A6"] = PoDoFo::PdfPageSize::A6;
  m_pageSizes["Letter"] = PoDoFo::PdfPageSize::Letter;
  m_pageSizes["Legal"] = PoDoFo::PdfPageSize::Legal;
  m_pageSizes["Tabloid"] = PoDoFo::PdfPageSize::Tabloid;
}


bool PixelToPdfWriter::AddPageAt(const std::string& pixelFile, uint width, uint height, uint index, 
                               PoDoFo::PdfPageSize pageSize, PoDoFo::PdfColorSpace)
{
  try
  {
    Rect pageRect = PdfPage::CreateStandardPageSize(pageSize);
    PdfPage& page = m_doc.GetPages().CreatePageAt(index, pageRect);

    PdfImageInfo info;
    info.Width = width; // Must be known beforehand and must be exact.
    info.Height = height;
    info.BitsPerComponent = 8;
    info.ColorSpace = PdfColorSpace::DeviceRGB; // Is always RGB from libinsane.

    ifstream ifs(pixelFile, ios::binary);
    string content;
    content.assign(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
    bufferview bv(content);

    const uint pageNumber = m_doc.GetPages().GetCount();
    std::unique_ptr<PdfImage> image =  m_doc.CreateImage("Page_" + to_string(pageNumber)); // No space.
    image->SetDataRaw(bv, info); // OK for pixel file, including LineArt
    double scale = std::min(pageRect.Width / image->GetWidth(), pageRect.Height / image->GetHeight());

    PdfPainter painter;
    painter.SetCanvas(page);
    painter.DrawImage(*(image), 0.0, 0.0, scale, scale);
    painter.FinishDrawing();
  }
  catch (PdfError& e)
  {
    cerr << e.ErrorMessage(e.GetCode()) << endl << e.what() << endl;
    return false;
  }
  
  return true;
}

void PixelToPdfWriter::Save(const std::string& pdfFile)
{
  m_doc.Save(pdfFile);
}

uint PixelToPdfWriter::GetNumberOfPages() const
{
  return m_doc.GetPages().GetCount();
}

void PixelToPdfWriter::RemovePageAt(uint index)
{
  m_doc.GetPages().RemovePageAt(index);
}
