// Copyright (C) 2015 Dave Griffiths
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

#include <QtGui>
#include <iostream>
#include <list>

#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow(red_king::model *model)
{
  m_model = model;

  m_Ui.setupUi(this);

  m_host_graph = new graph_widget();
  m_host_graph->init(120,m_model->get_host_cost(),25);
  m_Ui.host_cost_holder->addWidget(m_host_graph);

  m_parasite_graph = new graph_widget();
  m_parasite_graph->init(120,m_model->get_parasite_cost(),25);
  m_Ui.parasite_cost_holder->addWidget(m_parasite_graph);

  m_matrix = new matrix_widget();
  m_matrix->init(120,m_model->get_matrix(),25);
  m_Ui.matrix_holder->addWidget(m_matrix);

}
