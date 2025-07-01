// /*
//  * File:   PixelToImageWriter.cpp
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 27 06 2025, 20:28
//  */

#include "PixelToImageWriter.h"
#include <StampWorker.h>
#include <fstream>
#include <DefsInsaneWidget.h>
#include <DefsStampWidget.h>

using namespace std;

bool PixelToImageWriter::Convert(const std::string& pixelFilePath,
                                int imageWidth, int imageHeight,
                                std::vector<StampDescriptor*> * descriptors,
                                int outputFormat, wxImage * image)
{
  UpdateExtensionsMap();
  wxImage * outImage = image;
  unique_ptr<wxImage> tmpImage;
  if (!image)
  {
    tmpImage = make_unique<wxImage>();
    outImage = tmpImage.get();
  }
  string raw;
  ifstream ifs(pixelFilePath, ios::binary);
  if (!ifs.good())
  {
    cerr << _("Failed to read raw file.") << endl;
    return false;
  }
  raw.assign(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());

  outImage->SetData((unsigned char*) raw.data(), imageWidth, imageHeight, true); // true +++
  if (descriptors)
  {
    for (StampDescriptor * descriptor : *descriptors)
    {
      if (descriptor)
      {
        StampWorker::StampBackground(*outImage, descriptor->image, descriptor->location);
      }
    }
  }
  
  switch (outputFormat)
  {
    case PNG:
      outImage->SaveFile(pixelFilePath + "." + Extensions[PNG], wxBITMAP_TYPE_PNG);
      break;
    case JPEG:
      outImage->SaveFile(pixelFilePath + "." + Extensions[JPEG], wxBITMAP_TYPE_JPEG);
      break;
    case TIFF:
      outImage->SaveFile(pixelFilePath + "." + Extensions[TIFF], wxBITMAP_TYPE_TIFF);
      break;
    case PNM:
      outImage->SaveFile(pixelFilePath + "." + Extensions[PNM], wxBITMAP_TYPE_PNM);
      break;
    case PDF:
      break;
    default:
      cerr << _("Unhandled output image format.") << endl;
      return false;
  };
  
  return true;
}
