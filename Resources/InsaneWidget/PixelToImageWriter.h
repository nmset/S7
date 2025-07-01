// /*
//  * File:   PixelToImageWriter.h
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 27 06 2025, 20:28
//  */

#ifndef PIXELTOIMAGEWRITER_H
#define PIXELTOIMAGEWRITER_H

#include "DefsInsaneWidget.h"
#include <string>
#include <vector>
#include <wx/wx.h>

struct StampDescriptor;

/**
 * Create an image file from a raw scanned file.\n
 * Optionally,\n
 *  - use an wxImage object from the application\n
 *  - blend a stamp image on the converted image.\n
 */
class PixelToImageWriter
{
  DECLARE_DYNAMIC_CLASS( PixelToImageWriter )
public:
  static bool Convert(const std::string& pixelFilePath,
            int imageWidth, int imageHeight, std::vector<StampDescriptor*> * descriptors,
            int outputFormat = PNG, wxImage * image = nullptr);
};

#endif // PIXELTOIMAGEWRITER_H
