// /*
//  * File:   Common.h
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 27 06 2025, 20:34
//  */

#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <map>

enum {PDF, PNG, JPEG, TIFF, PNM};
typedef std::map <int, std::string> ExtensionsMap;

static ExtensionsMap Extensions;

static void UpdateExtensionsMap()
{
  Extensions[PDF] = "pdf";
  Extensions[PNG] = "png";
  Extensions[JPEG] = "jpeg";
  Extensions[TIFF] = "tiff";
  Extensions[PNM] = "pnm";
}

#endif // COMMON_H
