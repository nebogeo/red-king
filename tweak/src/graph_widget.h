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

#pragma once

#include <QtGui>
#include <QWidget>
#include "model/model.h"

class graph_widget : public QWidget
{
    Q_OBJECT
 public:

    graph_widget();

    void init(int graph_size, rk_real *data, int size);
    void recalc();

 protected:
    void paintEvent(QPaintEvent *event);
 signals:

 public slots:

 private:
  rk_real *m_data;
  int m_size;
  int m_graph_size;

  rk_real m_scale,m_offset,m_min,m_max;
};
