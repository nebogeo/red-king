#include "graph.h"
#include <iostream>

using namespace std;

graph_widget::graph_widget()
{
}

void graph_widget::init(int graph_size, rk_real *data, int size) {
  m_data=data;
  m_size=size;
  m_graph_size=graph_size;

  m_min=1e32;
  m_max=0;
  for (int i=0; i<m_size; i++) {
    if (m_data[i]<m_min) m_min = m_data[i];
    if (m_data[i]>m_max) m_max = m_data[i];
  }

  m_scale = m_graph_size/(m_max-m_min);
  m_offset = m_min;


}

void graph_widget::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);

    //create a black pen that has solid line
    //and the width is 2.
    QPen myPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(myPen);

    char txt[256];
    sprintf(txt,"%f",m_max);
    painter.drawText(QPoint(2, 12), txt);
    sprintf(txt,"%f",m_min);
    painter.drawText(QPoint(2, m_graph_size-3), txt);
    painter.drawRect(0,0,m_graph_size,m_graph_size);

    for (int i=0; i<m_size; i++) {
      painter.drawPoint(i*m_graph_size/(float)m_size,
                        m_graph_size-((m_data[i]-m_offset)*m_scale));
    }
}
