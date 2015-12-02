#include "canvas.h"

canvas::canvas() :
m_position(0)
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
  timer->start(100);

  setAttribute(Qt::WA_OpaquePaintEvent);
}

void canvas::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);

    //create a black pen that has solid line
    //and the width is 2.
    QPen myPen(Qt::white, 2, Qt::SolidLine);
    painter.setPen(myPen);

    for (int i=0; i<50; i++) {
      //draw a point
      double v = m_parasites[i];
      if (v>1.0) v=1;
      if (v<0.0) v=0;

      double h = m_hosts[i];
      if (h>1.0) h=1;
      if (h<0.0) h=0;

      myPen.setColor(QColor(v*255,0,h*255));
      painter.setPen(myPen);
      painter.drawPoint(i*5,m_position);
    }

    m_position++;

    if (m_position>250) m_position=0;
}
