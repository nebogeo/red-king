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

#include <iostream>
#include <jellyfish/core/types.h>
#include <jellyfish/audio.h>
#include "model/model.h"
#include "synth/additive_synth.h"


#include <unistd.h>
using namespace std;

audio_device *audio = NULL;

#define BANDS 100
additive_synth synth(BANDS);
additive_synth synth2(BANDS);
int frame=0;

void run_audio(void* c, unsigned int frames) {
    audio->left_out.zero();
    red_king::model *s = (red_king::model *)c;
    s->step();

    for (u32 i=0; i<50; i++) {
        synth2.set_level(i%BANDS,s->get_hosts()[i]);
        synth.set_level(i%BANDS,s->get_parasites()[i]);
        //synth.set_level(i,0.5+0.5*sin((i+(frame*10))/20.0));
    }
    frame++;

    synth.render(audio->left_out);
    synth2.render(audio->right_out);

    //audio->maybe_record();

    //s->process(frames,a->left_out.get_non_const_buffer());
//    sleep(1);


}


int main() {
  red_king::model m;

  srand(::time(NULL));

  audio = new audio_device("evolution",44100,2048);
  audio->m_client.set_callback(run_audio, &m);
  //audio->start_recording("evol_out");

  synth.set_freq(0.01);
  synth2.set_freq(0.01);


  for (;;) { sleep(1); }

  return 1;
}
