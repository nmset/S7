/*
* File:   PopupTransientWindow.cpp
* Author: Saleem EDAH-TALLY - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem EDAH-TALLY - © 2017
*
* Created on 3 mars 2017, 19:45
*/

#include "PopupTransientWindow.h"

IMPLEMENT_CLASS ( PopupTransientWindow, wxPopupTransientWindow )

PopupTransientWindow::PopupTransientWindow()
  : wxPopupTransientWindow()
{}

PopupTransientWindow::PopupTransientWindow ( wxWindow* parent, int flags )
  : wxPopupTransientWindow ( parent, flags )
{}

PopupTransientWindow::~PopupTransientWindow()
{}

void PopupTransientWindow::Dismiss()
{
  Destroy();
}

