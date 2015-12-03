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

#define BANDS 50

void run_audio(void* c, unsigned int frames) {
    app *a = (app *)c;

    a->m_audio->left_out.zero();
    a->m_model.step();

    for (u32 i=0; i<BANDS; i++) {
      a->m_synth_left.set_level(i%BANDS,a->m_model.get_hosts()[i]);
      a->m_synth_right.set_level(i%BANDS,a->m_model.get_parasites()[i]);
    }

    a->m_synth_left.render(a->m_audio->left_out);
    a->m_synth_right.render(a->m_audio->right_out);

    a->m_audio->maybe_record();
    //s->process(frames,a->left_out.get_non_const_buffer());
}

app::app() :
  m_synth_left(BANDS),
  m_synth_right(BANDS)
{
  srand(100);//::time(NULL));


  /*  m_audio = new audio_device("evolution",44100,2048);
  m_audio->m_client.set_callback(run_audio, this);
  m_audio->start_recording("evol_out");

  m_synth_left.set_freq(0.001);
  m_synth_right.set_freq(0.001);*/
}

void app::init_qt() {
  m_main_window = new MainWindow();
  m_canvas = new canvas();
  m_main_window->show();
  m_canvas->m_model = &m_model;
  m_main_window->m_Ui.canvas_holder->addWidget(m_canvas);
  m_main_window->m_model = &m_model;
}
