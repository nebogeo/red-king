#include "canvas.h"
#include <iostream>

using namespace std;

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

    m_model->step();

    //create a black pen that has solid line
    //and the width is 2.
    QPen myPen(Qt::white, 10, Qt::SolidLine);
    painter.setPen(myPen);

    for (int i=0; i<25; i++) {
      //draw a point
      double v = m_model->get_parasites()[i];

      if (v>1.0) v=1;
      if (v<0.0) v=0;

      double h = m_model->get_hosts()[i];
      if (h>1.0) h=1;
      if (h<0.0) h=0;

      //cerr<<v<<" "<<h<<endl;

      myPen.setColor(QColor(v*255,0,h*255));
      painter.setPen(myPen);
      painter.drawPoint(i*10,m_position);
    }

    myPen.setColor(QColor(0,0,0));
    painter.setPen(myPen);
    for (int i=0; i<25; i++) {
      painter.drawPoint(i*10,m_position+4);
    }
    m_position+=4;

    if (m_position>500) m_position=0;

}
