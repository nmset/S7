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

#include "Common.h"
#include <string>
#include <wx/wx.h>

/**
 * Create an image file from a raw scanned file.\n
 */
class PixelToImageWriter
{
public:
  static bool Convert(const std::string& pixelFilePath,
            int imageWidth, int imageHeight, int outputFormat = PNG,
            wxImage * image = nullptr);
};

#endif // PIXELTOIMAGEWRITER_H
