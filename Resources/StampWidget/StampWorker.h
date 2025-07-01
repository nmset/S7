// /*
//  * File:   StampWorker.h
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 02 07 2025, 21:23
//  */

#ifndef STAMPWORKER_H
#define STAMPWORKER_H

#include <wx/wx.h>
#include "DefsStampWidget.h"

struct StampDescriptor;

/**
 * A stamp is understood here as\n
 *  - a transparent text in a transparent frame with no borders\n
 *  - an opaque text on an opaque background with no borders.\n
 * 
 * The text may be rotated. Actually, an initial image with the text is rotated
 * and its new orthogonal bounds accepted (fortunately, we don't have to compute
 * that).\n
 * The font point size is rescaled to match the scan resolution. If the
 * scanResolution parameter is invalid (<=0), the font is not rescaled.
 */
class StampWorker
{
  DECLARE_DYNAMIC_CLASS( StampWorker )
public:
  static wxImage CreateStamp(StampDescriptor * descriptor, int scanResolution = -1);
  static void StampBackground(wxImage& background, const wxImage& stamp, int location = CENTRE);

};

#endif // STAMPWORKER_H
