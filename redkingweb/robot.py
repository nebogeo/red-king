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
from sim import blip
import sim.twitter

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
        #time.sleep(1)

    return out,th

# time in seconds, step in samples
def render_blipsim(model,blip,time_length):
    sim_length = int(time_length*44100)
    out = np.zeros(sim_length,dtype=np.float32)
    steps = sim_length/blip.bar_length/2
    skip = 20
    th = thumb.thumb(steps*skip,model.size(),10)
    pre_run = 100
    for i in range(0,pre_run):
        model.step()
        time.sleep(0.3)

    #blip.update(parasite_state_array(model))
    #if len(blip.blips)<2: return False,False
    blip.update(host_state_array(model))
    if len(blip.blips)<2: return False,False

    for i in range(0,steps):
        blip.update(parasite_state_array(model))
        blip.render(out)
        blip.update(host_state_array(model))
        blip.render(out)
        for i in range(0,skip):
            th.render(model)
            model.step()
        time.sleep(0.3)

    return out,th

def run(location):
    length = 20
    cp = random_cp()

    # cp.amin = 5.2926940918
    # cp.amax = 7.14304304123
    # cp.a_p = 4.15711736679
    # cp.betmin = 4.14748334885
    # cp.bemaxtime = 4.37798070908
    # cp.beta_p = 3.67618966103
    # cp.g = -3.1736767292
    # cp.h = 0.39003816246

    params_str = cp_to_str(cp)
    base_name = hashlib.md5(params_str).hexdigest()
    m = redking.model()

    m.set_model(1)
    m.m_pstart = cp.pstart;
    m.m_hstart = cp.hstart;

    m.m_cost_params=cp
    m.init()
    #s = synth.synth(m.size())
    s = blip.blip(m.size(),0.25)
    #out,th = render_sim(m,s,length,1000)
    out,th = render_blipsim(m,s,length)
    if th!=False:
        wavname = location+base_name+".wav"
        imgname = location+base_name+".png"
        scipy.io.wavfile.write(wavname,44100,out)
        os.system("oggenc "+wavname)
        #os.system("aplay "+wavname)
        os.system("rm "+wavname)
        th.save(imgname)
        os.system("mogrify -resize 600% -filter Point "+imgname)
        d=Sim(created_date = timezone.now(),
            base_name = base_name,
            length = length,
            params = params_str)
        d.save()
        sim.twitter.tweet("I just generated a new host/parasite evolution sim: http://redking.fo.am/sim/"+str(d.id),imgname,sim.twitter.api)

while(True):
    run("media/sim/")
