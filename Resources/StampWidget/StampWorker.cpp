// /*
//  * File:   StampWorker.cpp
//  * Author: Saleem Edah-Tally - nmset@yandex.com
//  * License : CeCILL-C
//  * Copyright Saleem Edah-Tally - © 2025
//  *
//  * Created on 02 07 2025, 21:23
//  */

#include "StampWorker.h"
#include <math.h> // M_PI

wxImage StampWorker::CreateStamp(StampDescriptor * descriptor, int scanResolution)
{
  /*
   * Rescale the font point size of the stamp according to the scan resolution.
   * Exclude rescaling on v4l that does not have a resolution parameter.
   */
  wxFont dcFont(descriptor->font);
  double scale = (scanResolution > 0)
              ? (double) scanResolution / 72.0
              :1.0;
  
  wxCoord extentWidth, extentHeight, textLineHeight;
  {
    wxMemoryDC dc;
    if (scale != 1.0)
      dcFont.SetFractionalPointSize(descriptor->font.GetFractionalPointSize() * scale);
    dc.SetFont(dcFont); // Mandatory despite set below.
    dc.GetMultiLineTextExtent(descriptor->text, &extentWidth, &extentHeight, &textLineHeight, &dcFont);
  }
  wxBitmap bmp;
  // bmp.UseAlpha(true); // Not mandatory.
  bmp.Create(extentWidth, extentHeight); // Memo: See CreateWithDIPSize
  wxImage img0, img;

  wxMemoryDC dc(bmp);
  dc.SetFont(dcFont);
  if (descriptor->transparent)
  {
    // +++, Use the red channel only: used by ConvertColourToAlpha().
    // The chosen value is inversely proportional to the transparency of the text itself.
    dc.SetTextForeground(wxColour(128, 0, 0)); // A good mean is reasonable.
    dc.SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);
  }
  else
  {
    dc.SetTextForeground(descriptor->foregroundColour);
    dc.SetTextBackground(descriptor->backgroundColour);
    dc.SetBackgroundMode(wxBRUSHSTYLE_SOLID);
  }
  
  dc.DrawText(descriptor->text, wxPoint(0, 0));
  
  img0 = bmp.ConvertToImage();
  if (!descriptor->transparent)
    img0.InitAlpha(); // So that there is no dark surrounding if it is rotated.

  img = img0.Rotate((M_PI / 180.0) * descriptor->rotationAngle, wxPoint(0, 0));
  if (descriptor->transparent)
  {
    img.ConvertColourToAlpha(descriptor->foregroundColour.GetRed(),
                            descriptor->foregroundColour.GetGreen(),
                            descriptor->foregroundColour.GetBlue()); // The target colour is passed in.
    img.ChangeSaturation(-0.7); // Slightly better looking.
  }
  
  return img;
}

void StampWorker::StampBackground(wxImage& background,
                                  const wxImage& stamp, int location)
{
  // Simply pasting the stamp on the background, i.e., the scanned page.
  wxPoint bgCentre(background.GetWidth() / 2, background.GetHeight() / 2);
  wxPoint stampCentre(stamp.GetWidth() / 2, stamp.GetHeight() / 2);
  int x = 0, y = 0;
  switch (location)
  {
  case NORTH:
    x = bgCentre.x - stampCentre.x;
    y = 0;
    break;
  case SOUTH:
    x = bgCentre.x - stampCentre.x;
    y = background.GetHeight() - stamp.GetHeight();
    break;
  case EAST:
    x = background.GetWidth() - stamp.GetWidth();
    y = bgCentre.y - stampCentre.y;
    break;
  case WEST:
    x = 0;
    y = bgCentre.y - stampCentre.y;
    break;
  case NORTH_EAST:
    x = background.GetWidth() - stamp.GetWidth();
    y = 0;
    break;
  case NORTH_WEST:
    x = 0;
    y = 0;
    break;
  case SOUTH_EAST:
    x = background.GetWidth() - stamp.GetWidth();
    y = background.GetHeight() - stamp.GetHeight();
    break;
  case SOUTH_WEST:
    x = 0;
    y = background.GetHeight() - stamp.GetHeight();
    break;
  default: // CENTRE
    x = bgCentre.x - stampCentre.x;
    y = bgCentre.y - stampCentre.y;
    break;
  }
  
  wxPoint stampLocation(x, y);
  background.Paste(stamp, stampLocation.x, stampLocation.y, wxIMAGE_ALPHA_BLEND_COMPOSE);
}

