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

#include <QtGui>
#include "ui_redking.h"
#include "../model/model.h"
#include "graph.h"
#include "../synth/additive_synth.h"
#include "matrix_widget.h"

#include <iostream>
#include <string>

using namespace std;
using namespace spiralcore;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(red_king::model *model);
    Ui_MainWindow m_Ui;

    red_king::model *m_model;
    additive_synth *m_synth_left;
    additive_synth *m_synth_right;

    pthread_mutex_t* m_mutex;

protected:

private slots:
  void slot_restart() {
    pthread_mutex_lock(m_mutex);
    m_model->init();
    m_synth_left->reset();
    m_synth_right->reset();
    pthread_mutex_unlock(m_mutex);
  }

  void slot_model(bool s) {
    if (s) m_model->set_model(1);
    else m_model->set_model(0);
    update_model();
  }

  void slot_amin(int v) {
    m_model->m_cost_params.amin = v/10.0;
    update_model();
  }

  void slot_amax(int v) {
    m_model->m_cost_params.amax = v/10.0;
    update_model();
  }

  void slot_umin(int v) {
    m_model->m_cost_params.umin = v/10.0;
    update_model();
  }

  void slot_umax(int v) {
    m_model->m_cost_params.umax = v/10.0;
    update_model();
  }

  void slot_ap(int v) {
    m_model->m_cost_params.a_p = v/10.0;
    update_model();
  }

  void slot_betmin(int v) {
    m_model->m_cost_params.betmin = v/10.0;
    update_model();
  }

  void slot_bemaxtime(int v) {
    m_model->m_cost_params.bemaxtime = v/10.0;
    update_model();
  }

  void slot_vmin(int v) {
    m_model->m_cost_params.vmin = v/10.0;
    update_model();
  }

  void slot_vmax(int v) {
    m_model->m_cost_params.vmax = v/10.0;
    update_model();
  }

  void slot_betap(int v) {
    m_model->m_cost_params.beta_p = v/10.0;
    update_model();
  }

  void update_model() {
    m_model->update_cost_functions();
    m_parasite_graph->recalc();
    m_host_graph->recalc();
    m_matrix->recalc();
  }

private:
  graph_widget *m_host_graph;
  graph_widget *m_parasite_graph;
  matrix_widget *m_matrix;

};
