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

import math
import numpy as np
import scipy.io.wavfile
import redking
import copy
import random
import simulation

class synth:
    def __init__(self,bands):
        self.freq = 0.01
        self.oscs = [0 for i in range(0,bands)]
        self.level = [0 for i in range(0,bands)]
        self.last_level = [0 for i in range(0,bands)]
        self.t = 0
        self.t_max = 10

    def update(self,level):
        self.last_level=copy.copy(self.level)
        self.level = copy.copy(level)
        self.t = 0

    def render(self):
        ret = 0
        t = self.t/float(self.t_max)
        for i in range(0,len(self.oscs)):
            l = self.level[i]*t + self.last_level[i]*(1-t)
            #l = self.level[i]
            ret += math.sin(self.oscs[i])*l
            self.oscs[i]+=self.freq*(i+1)
        self.t+=1
        return ret*0.01

def compare(a,b):
    if len(a)!=len(b): return -1
    ret = 0
    for i in range(0,len(a)):
        ret += a[i]-b[i]
    return abs(ret)

def grain_render(m,s,frames,buf,pos,step):
    env = 0
    env_size=20
    last = []
    max_change = 0
    av_change = 0
    s.t_max=step
    for i in range(0,frames):
        if i<env_size: env=i/float(env_size)
        elif i>frames-env_size: env=(frames-i)/float(env_size)
        else: env=1
        buf[pos]+=s.render()*env
        pos+=1
        if i%step==0:
            #print(out[i])
            t = combined_array(m)
            #diff = compare(t,last)
            #av_change = (av_change+diff)/2.0
            #if diff>max_change: max_change=diff
            #print(i,av_change,max_change)
            #s.level = copy.copy(t)
            last = t
            s.update(t)
            m.step()

def path(m,s,filename,grains,frames,step,overlap):
    out = np.zeros(frames*grains,dtype=np.float32)
    pos = 0
    for i in range(0,grains):
        v = i/float(grains)
        #v = v*0.5+0.5 # second half
        #m.m_cost_params.g=(v*30.0)-5.0
        #m.m_cost_params.beta_p=1.8+(1-v)
        m.init()
        print(i,m.m_cost_params.g)
        grain_render(m,s,frames,out,pos,step)
        pos = i*frames*(1-overlap)
        scipy.io.wavfile.write(filename,44100,out)

def gridpath(m,s,filename,grainsx,grainsy,frames,step,overlap):
    out = np.zeros(frames*grainsx*grainsy,dtype=np.float32)
    for j in range(0,grainsy):
        jv = j/float(grainsy)
        pos = frames*(grainsx+grainsy*j)*overlap
        for i in range(0,grainsx):
            v = i/float(grainsx)
            #v = v*0.5+0.5 # second half
            m.m_cost_params.g=(v*30.0)-5.0
            m.m_cost_params.beta_p=1.8+(1-jv)
            #m.m_cost_params.beta_p=beta_p
            m.init()
            print(i,m.m_cost_params.g,m.m_cost_params.beta_p)
            grain_render(m,s,frames,out,pos,step)
            pos = i*frames*(1-overlap)
            scipy.io.wavfile.write(filename,44100,out)

def blendpath(m,a,b,s,filename,grains,frames,step,overlap):
    print(frames,grains,overlap,frames*grains*(1-overlap))

    out = np.zeros(frames*(2+grains)*(1-overlap),dtype=np.float32)
    pos = 0
    for i in range(0,grains):
        v = i/float(grains)
        m.m_cost_params=blend_cp(a,b,v)
        m.init()
        print(i,pos,len(out),i*frames*(1-overlap))
        grain_render(m,s,frames,out,pos,step)
        pos = i*frames*(1-overlap)
        scipy.io.wavfile.write(filename,44100,out)


def big_seed(i):
    random.seed(i)
    a = random_cp()
    b = random_cp()
    print("----------------- "+str(i))
    print_cp(a)
    print("->")
    print_cp(b)
    print("")
    #blendpath(m,a,b,s,"random_path-"+str(i)+"-0-big.wav",2500,6000,10,0.75)

m = redking.model()
#m.set_model(0)
# m.m_cost_params.amin = 0
# m.m_cost_params.amax = 5.4
#m.m_cost_params.a_p = 2.4
# m.m_cost_params.betamin = 0
# m.m_cost_params.bemaxtime = 9.3
#m.m_cost_params.beta_p = 2.9
# m.m_cost_params.g = -0.2
# m.m_cost_params.h = 0.4

print m.m_cost_params.a_p

#m.m_cost_params.a_p=1.9
#m.m_cost_params.a_p=0.4

#m.m_cost_params.beta_p=0.3
#m.m_cost_params.beta_p=0.38

#m.init()
s = synth(m.size())

#path(m,s,"test12.wav",20,2000,10,0.75)

#path(m,s,"single-run4.wav",1,250000,10,0)

#gridpath(m,s,"test10.wav",20,20,6000,10,0.5)



# for i in range(0,300):
#     random.seed(i)
#     a = random_cp()
#     b = random_cp()
#     print_cp(a)
#     print_cp(b)
#     blendpath(m,a,b,s,"random_path-"+str(i)+".wav",20,6000,10,0.75)

big_seed(21)
big_seed(23)
big_seed(38)
big_seed(62)


# 5 frequency decrease with overlap (g=-5 to 25)
# 6 frequency increase (g=-5 to 25)
# 7 zooming on second half (g=10-25) - interesting bits
# 8 double length - 4 times overlap (0.75), otherwise same as 7
# 9 grid (g=-5 to 25, beta_p=2.8 to 1.8)
# 10 stereo grid for 9 with a_p = 1.9
