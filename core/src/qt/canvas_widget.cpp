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

#include "canvas_widget.h"
#include <iostream>
#include "../model/types.h"

using namespace std;

canvas_widget::canvas_widget() :
m_position(0)
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
  timer->start(100);

  setAttribute(Qt::WA_OpaquePaintEvent);
}

void canvas_widget::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);

    m_model->step();

    //create a black pen that has solid line
    //and the width is 2.
    QPen myPen(Qt::white, 10, Qt::SolidLine);
    painter.setPen(myPen);

    for (int i=0; i<N; i++) {
      //draw a point
      double v = log10(m_model->get_parasites()[i])*50.0;

      if (v>1.0) v=1;
      if (v<0.0) v=0;

      double h = log10(m_model->get_hosts()[i])*50.0;
      if (h>1.0) h=1;
      if (h<0.0) h=0;


      myPen.setColor(QColor(v*255,0,h*255));
      painter.setPen(myPen);
      painter.drawPoint(i*10,m_position);
    }

    myPen.setColor(QColor(0,0,0));
    painter.setPen(myPen);
    for (int i=0; i<N; i++) {
      painter.drawPoint(i*10,m_position+4);
    }
    m_position+=4;

    if (m_position>500) m_position=0;

}
