#!/usr/bin/env python
# Red King Simulation Sonification
# Copyright (C) 2016 Foam Kernow
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import os
import math
import time
import hashlib
import numpy as np
import scipy.io.wavfile
import sim.redking
import random
from sim.simulation import *
from sim import thumb
from sim import synth

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "redkingweb.settings")

import django
import datetime
from redking.models import *
from django.utils import timezone
from django.db.models import Max, Count, Sum

django.setup()

# time in seconds, step in samples
def render_sim(model,synth,time_length,synth_step):
    sim_length = time_length*44100
    out = np.zeros(sim_length,dtype=np.float32)
    steps = sim_length/synth_step
    synth.t_max=synth_step
    pos = 0
    th = thumb.thumb(steps,model.size(),10)
    for i in range(0,steps):
        for p in range(0,synth_step):
            out[pos]=synth.render()
            pos+=1
        th.render(model)
        synth.update(combined_array(model))
        model.step()
        #time.sleep(0.3)

    return out,th

def run(location):
    length = 10
    cp = random_cp()
    params_str = cp_to_str(cp)
    base_name = hashlib.md5(params_str).hexdigest()
    m = redking.model()
    m.m_cost_params=cp
    m.init()
    s = synth.synth(m.size())
    out,th = render_sim(m,s,length,100)
    wavname = location+base_name+".wav"
    scipy.io.wavfile.write(wavname,44100,out)
    os.system("oggenc "+wavname)
    os.system("rm "+wavname)
    th.save(location+base_name+".png")
    Sim(created_date = timezone.now(),
        base_name = base_name,
        length = length,
        params = params_str).save()

while 1:
    run("media/sim/")
