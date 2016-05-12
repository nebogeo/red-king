// Copyright (C) 2015 Foam Kernow
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "matrix_widget.h"
#include <iostream>

using namespace std;

matrix_widget::matrix_widget()
{
}

void matrix_widget::init(int graph_size, rk_real **data, int size) {
  m_data=data;
  m_size=size;
  m_graph_size=graph_size;
  recalc();
}

void matrix_widget::recalc() {
  m_min=1e32;
  m_max=0;
  for (int i=0; i<m_size; i++) {
    for (int j=0; j<m_size; j++) {
      if (m_data[i][j]<m_min) m_min = m_data[i][j];
      if (m_data[i][j]>m_max) m_max = m_data[i][j];
    }
  }
  m_scale = m_graph_size/(m_max-m_min);
  m_offset = m_min;
  repaint();
}


void matrix_widget::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);

    //create a black pen that has solid line
    //and the width is 2.
    QPen myPen(Qt::black, 6, Qt::SolidLine);
    painter.setPen(myPen);

    for (int i=0; i<m_size; i++) {
      for (int j=0; j<m_size; j++) {
        int v = abs((int)m_data[i][j]*10);
        myPen.setColor(QColor(v%255,v%255,v%255));
        painter.setPen(myPen);
        painter.drawPoint(i*m_graph_size/(float)m_size,
                          j*m_graph_size/(float)m_size);
      }
    }


    char txt[256];
    sprintf(txt,"%f",m_max);
    painter.drawText(QPoint(2, m_graph_size+12+10), txt);
    sprintf(txt,"%f",m_min);
    painter.drawText(QPoint(2, (m_graph_size-3)+m_graph_size+10), txt);
    QPen myPen2(Qt::black, 1, Qt::SolidLine);
    painter.setPen(myPen2);
    painter.drawRect(0,0,m_graph_size,m_graph_size);
    painter.drawRect(0,m_graph_size+10,m_graph_size,m_graph_size);

    for (int j=0; j<m_size; j+=5) {
      int x,y=0;
      int lx = 0;
      int ly = (m_graph_size-((m_data[0][j]-m_offset)*m_scale)
                +m_graph_size)+10;

      for (int i=0; i<m_size; i++) {
        x=i*m_graph_size/(float)m_size;
        y=(m_graph_size-((m_data[i][j]-m_offset)*m_scale)
           +m_graph_size)+10;
        painter.drawLine(x,y,lx,ly);
        lx=x;
        ly=y;
      }
    }


}
