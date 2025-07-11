/*
* File:   MiscTools.h
* Author: Saleem EDAH-TALLY - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem EDAH-TALLY - © 2017
*
* Created on 4 mars 2017, 11:40
*/

#ifndef MISCTOOLS_H
#define MISCTOOLS_H

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/popupwin.h>

/**
 * Miscellaneous static functions.
 */
class MiscTools
{
public:
  MiscTools();
  virtual ~MiscTools();

  /**
   * Saves size and position of a wxWindow.
   */
  static void SaveSizePos ( wxConfig * config, wxWindow * wind, const wxString& pathPrefix );

  /**
   * Restores size and position of a wxWindow.
   */
  static void RestoreSizePos ( wxConfig * config, wxWindow * wind, const wxString& pathPrefix );

  /**
   * Shows a transient popup at mouse cursor.
   * Position the popup relative to screen edges and popup dimensions,
   * if possible.
   */
  static void ShowTransientPopup ( wxPopupTransientWindow * p, wxWindow* content,
                                   const int width = 0, const int height = 0 );

  /**
   * Shows a message as a modal dialog or as a notification.
   */
  static void MessageBox ( const wxString& msg, const bool notify = false );
  static void AsyncMessageBox ( const wxString& msg, const bool notify = false );

  /**
   * Creates a validator excluding file name forbidden characters, path
   * separator and optionally space character.
   */
  static wxTextValidator* MakeFileNameValidator ( bool excludeSpace = true );

private:

};

#endif /* MISCTOOLS_H */

