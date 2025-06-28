/*
* File:   PopupTransientWindow.h
* Author: Saleem EDAH-TALLY - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem EDAH-TALLY - © 2017
*
* Created on 3 mars 2017, 19:45
*/

#ifndef POPUPTRANSIENTWINDOW_H
#define POPUPTRANSIENTWINDOW_H

#include <wx/wx.h>
#include <wx/popupwin.h>

/** No further functionality provided here on top of wxPopupTransientWindow.\n
 * Just calls Destroy() on dismiss.
 */
class PopupTransientWindow : public wxPopupTransientWindow
{
  DECLARE_CLASS ( PopupTransientWindow )
public:
  PopupTransientWindow();
  PopupTransientWindow ( wxWindow *parent, int flags = wxBORDER_NONE );
  virtual ~PopupTransientWindow();

  void Dismiss() override;

private:
};

#endif /* POPUPTRANSIENTWINDOW_H */

