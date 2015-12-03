#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtGui>
#include <QWidget>
#include "../model/model.h"

class canvas : public QWidget
{
    Q_OBJECT
 public:

    canvas();
    red_king::model *m_model;

 protected:
   void paintEvent(QPaintEvent *event);
 signals:

 public slots:
   void animate() { repaint(); }

 private:
  int m_position;
};

#endif // MYWIDGET_H
