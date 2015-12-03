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

    char txt[256];
    sprintf(txt,"%f",m_max);
    painter.drawText(QPoint(2, 12), txt);
    sprintf(txt,"%f",m_min);
    painter.drawText(QPoint(2, m_graph_size-3), txt);
    painter.drawRect(0,0,m_graph_size,m_graph_size);

    for (int i=0; i<m_size; i++) {
      for (int j=0; j<m_size; j++) {
        int v = abs(m_data[i][j]*10);
        myPen.setColor(QColor(v%255,v%255,v%255));
        painter.setPen(myPen);
        painter.drawPoint(i*m_graph_size/(float)m_size,
                          j*m_graph_size/(float)m_size);
      }
    }

}
