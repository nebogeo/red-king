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
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA

#include "app.h"

using namespace red_king;

#define BANDS 25

void run_audio(void* c, unsigned int frames) {
    app *a = (app *)c;

    a->m_audio->left_out.zero();

    pthread_mutex_lock(a->m_mutex);
    a->m_model.step();

    for (u32 i=0; i<BANDS; i++) {
           a->m_synth_left.set_level(i%BANDS,a->m_model.get_hosts()[i]+
                              a->m_model.get_parasites()[i]);
      a->m_synth_left.set_level(i%BANDS,a->m_model.get_hosts()[i]);
      a->m_synth_right.set_level(i%BANDS,a->m_model.get_parasites()[i]);
    }
    pthread_mutex_unlock(a->m_mutex);

    a->m_synth_left.render(a->m_audio->left_out);
    //a->m_synth_right.render(a->m_audio->right_out);

    //a->m_audio->maybe_record();
    //s->process(frames,a->left_out.get_non_const_buffer());
}

app::app() :
  m_synth_left(BANDS),
  m_synth_right(BANDS)
{
  m_mutex = new pthread_mutex_t;
  pthread_mutex_init(m_mutex,NULL);

  m_audio = new audio_device("evolution",44100,2048*4);
  m_audio->m_client.set_callback(run_audio, this);
  //m_audio->start_recording("evol_out");

  m_synth_left.set_freq(0.01);
  m_synth_right.set_freq(0.01);
}

void app::init_qt() {
  m_main_window = new MainWindow(&m_model);
  m_main_window->m_mutex = m_mutex;
  m_main_window->m_synth_left = &m_synth_left;
  m_main_window->m_synth_right = &m_synth_right;

  m_canvas = new canvas();
  m_main_window->show();
  m_canvas->m_model = &m_model;
  m_main_window->m_Ui.canvas_holder->addWidget(m_canvas);

  qt_controller c;
  c = build_param_controller("AMIN",0,200,m_main_window->m_Ui.host_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_amin(int)));
  //  QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_amin(double)));

  c = build_param_controller("AMAX",0,200,m_main_window->m_Ui.host_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_amax(int)));
  //  QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_amax(double)));

  c = build_param_controller("UMIN",0,200,m_main_window->m_Ui.host_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_umin(int)));
  // QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_umin(double)));

  c = build_param_controller("UMAX",0,200,m_main_window->m_Ui.host_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_umax(int)));
  // QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_uax(double)));

  c = build_param_controller("A_P",-10,200,m_main_window->m_Ui.host_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_ap(int)));
  // QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_ap(double)));



  c = build_param_controller("BETMIN",0,200,m_main_window->m_Ui.parasite_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_betmin(int)));
  // QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_betmin(double)));
  c = build_param_controller("BEMAXTIME",0,200,m_main_window->m_Ui.parasite_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_bemaxtime(int)));
  // QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_bemaxtime(double)));
  c = build_param_controller("VMIN",0,200,m_main_window->m_Ui.parasite_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_vmin(int)));
  //  QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_vmin(double)));
  c = build_param_controller("VMAX",0,200,m_main_window->m_Ui.parasite_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_vmax(int)));
  // QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_vmax(double)));
  c = build_param_controller("BETA_P",-10,200,m_main_window->m_Ui.parasite_params_holder);
  QObject::connect(c.m_dial, SIGNAL(valueChanged(int)), m_main_window, SLOT(slot_betap(int)));
  // QObject::connect(c.m_spin, SIGNAL(valueChanged(double)), m_main_window, SLOT(slot_betap(double)));


}


app::qt_controller app::build_param_controller(const QString &name, int low, int high, QLayout *parent) {

  QWidget *mainWidget = new QWidget();
  mainWidget->setGeometry(QRect(330, 390, 127, 54));
  QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  QHBoxLayout *innerLayout = new QHBoxLayout();
  QLabel *label = new QLabel(mainWidget);
  label->setText(name);
  innerLayout->addWidget(label);

  //  QDoubleSpinBox *spin = new QDoubleSpinBox(mainWidget);
  // innerLayout->addWidget(spin);

  mainLayout->addLayout(innerLayout);
  QSlider *dial = new QSlider(Qt::Horizontal,mainWidget);
  dial->setRange(low, high);
  mainLayout->addWidget(dial);

  parent->addWidget(mainWidget);

  qt_controller ret;
  //  ret.m_spin = spin;
  ret.m_dial = dial;
  return ret;



}
