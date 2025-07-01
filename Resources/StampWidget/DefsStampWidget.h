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

#include <wx/wx.h>

// ----------------------------------------------------------------------------
enum {CENTRE = 0, NORTH, SOUTH, EAST, WEST,
  NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};

struct StampDescriptor
{
  wxImage image;
  wxString text;
  wxFont font = wxNullFont;
  wxColour foregroundColour = wxNullColour;
  wxColour backgroundColour = wxNullColour;
  int rotationAngle = 45;
  int location = 0;
  bool transparent = true;
};
#endif // COMMON_H
