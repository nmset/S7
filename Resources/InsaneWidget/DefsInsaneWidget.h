// /*
//  * File:   DefsInsaneWidget.h
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 08 07 2025, 20:44
//  */

#ifndef DEFSINSANEWIDGET_H
#define DEFSINSANEWIDGET_H

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

#endif // DEFSINSANEWIDGET_H
