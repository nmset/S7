/*
* File:   TimeredStatusBar.h
* Author: Saleem EDAH-TALLY - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem EDAH-TALLY - © 2017
*
* Created on April 22, 2018, 3:55 PM
*/

#ifndef TIMEREDSTATUSBAR_H
#define TIMEREDSTATUSBAR_H

#include <wx/wx.h>
#include <wx/timer.h>

/**
 * We want to show a transient message.\n
 * The widget show also CAPS and NUM status.\n
 */
class TimeredStatusBar : public wxStatusBar
{
public:
  TimeredStatusBar ( wxWindow * parent );
  virtual ~TimeredStatusBar();

  /**
   * Called by the application to display 'text' transiently.
   */
  void SetTransientText ( const wxString& text, int duration = 3000);
  void OnIdle ( wxIdleEvent& evt );
private:
  uint m_numFields = 3;
  wxTimer m_timer;
  void OnTimer ( wxTimerEvent& evt );
};

#endif /* TIMEREDSTATUSBAR_H */

