#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtGui>
#include <QWidget>

class canvas : public QWidget
{
    Q_OBJECT
 public:

    canvas();
    double *m_parasites;
    double *m_hosts;

 protected:
    void paintEvent(QPaintEvent *event);
 signals:

 public slots:
   void animate() { repaint(); }

 private:
  int m_position;
};

#endif // MYWIDGET_H
