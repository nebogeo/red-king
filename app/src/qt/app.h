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

#include <jellyfish/core/types.h>
#include <jellyfish/audio.h>
#include "../model/model.h"
#include "../synth/additive_synth.h"
#include <QtGui>
#include "MainWindow.h"
#include "canvas.h"

#include <pthread.h>

namespace red_king {

class app {
 public:
  app();
  void init_qt();

  audio_device *m_audio;
  additive_synth m_synth_left;
  additive_synth m_synth_right;

  model m_model;

  MainWindow *m_main_window;
  canvas *m_canvas;
  pthread_mutex_t* m_mutex;

 private:

  class qt_controller {
  public:
    QWidget *m_dial;
    QWidget *m_spin;
  };

  qt_controller build_param_controller(const QString &name, int low, int high, QLayout *parent);


};

};
