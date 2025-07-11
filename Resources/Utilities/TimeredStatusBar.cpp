/*
* File:   TimeredStatusBar.cpp
* Author: Saleem EDAH-TALLY - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem EDAH-TALLY - © 2017
*
* Created on April 22, 2018, 3:55 PM
*/

#include "TimeredStatusBar.h"

TimeredStatusBar::TimeredStatusBar ( wxWindow * parent)
  : wxStatusBar ( parent, wxID_ANY )
{
  m_numFields = 3;
  /* We use default reasonable widths for CAPS and NUM. */
  const int widths[3] = {-1, 70, 90};
  SetFieldsCount ( m_numFields, widths );
  m_timer.Stop();
  m_timer.SetOwner ( this );
  Bind ( wxEVT_TIMER, &TimeredStatusBar::OnTimer, this );
  Bind ( wxEVT_IDLE, &TimeredStatusBar::OnIdle, this );
}

TimeredStatusBar::~TimeredStatusBar()
{
}

void TimeredStatusBar::SetTransientText ( const wxString& text, int duration )
{
  wxStatusBar::SetStatusText ( text, 0 );
  m_timer.Start ( duration, wxTIMER_ONE_SHOT );
}

void TimeredStatusBar::OnTimer ( wxTimerEvent& evt )
{
  wxStatusBar::SetStatusText ( wxEmptyString, 0 );
}

void TimeredStatusBar::OnIdle ( wxIdleEvent& evt )
{
  if ( wxGetKeyState ( WXK_NUMLOCK ) )
  {
    SetStatusText (_("NUM"), m_numFields - 2 );
  }
  else
  {
    SetStatusText ( wxEmptyString, m_numFields - 2 );
  }
  if ( wxGetKeyState ( WXK_CAPITAL ) )
  {
    SetStatusText (_("CAPS"), m_numFields - 1 );
  }
  else
  {
    SetStatusText ( wxEmptyString, m_numFields - 1 );
  }
}
