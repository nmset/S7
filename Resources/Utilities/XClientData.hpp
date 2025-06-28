/*
* File:   XClientData.hpp
* Author: Saleem Edah-Tally - nmset@yandex.com
* License: CeCILL-C
* Copyright Saleem Edah-Tally - © 2024
*
* Created on ?
*/

#ifndef XCLIENTDATA_H
#define XCLIENTDATA_H

#include <wx/wx.h>

/**
 * For the available scanner list.
 */
class XClientData : public wxClientData
{
public :
  XClientData ( const wxVariant& data ) : wxClientData()
  {
    m_data = data;
  }
  virtual ~XClientData() {};

  wxVariant GetData() const
  {
    return m_data;
  }
private:
  wxVariant m_data;
};

#endif // XCLIENTDATA_H
