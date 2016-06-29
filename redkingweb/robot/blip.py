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

import math
import numpy as np
import iso226
import copy
import techno
import strain
import time

def pitch(note):
    return math.pow(2,(note-69)/12.0)*440

class blip:
    def __init__(self,bands,bar_length):
        self.level = [0 for i in range(0,bands)]
        self.blips = []
        self.events = []
        self.bar_length = int(bar_length*44100)
        self.pos = 0

    def update(self,level):
        self.blips=strain.find_centres(level)

    def render(self,out,mode):
        if len(self.blips)>0:
            step = self.bar_length/len(self.blips)
            for i,b in enumerate(self.blips):
                # midi note to frequency
                p = pitch(b[0]+69)*4
                # make an event for this note
                self.events.append({'pos':i*step,
                                    'freq':p,
                                    'tec':techno.techno(0.3+b[1]*0.5,0.4),
                                    'vol':iso226.iso226(90,p)})
            if mode=="TECHNO": env = 150
            else: env = 50
            print(self.blips)
            for i in range(0,self.bar_length):
                if self.pos<len(out):
                    for e in self.events:
                        if mode=="TECHNO":
                            s = 0.016*e['tec'].generate(self.pos/44100.0*e['freq'])*e['vol']
                        else:
                            s = 0.008*math.sin(self.pos/44100.0*e['freq'])*e['vol']

                        if i>e['pos'] and i<=e['pos']+env:
                            env_lev = 1-(e['pos']+env-i)/float(env)
                            out[self.pos] += s*env_lev
                        if i>e['pos']+env:
                            out[self.pos] += s
                            e['vol']*=0.9995
                    self.pos+=1
                if i%50==0: time.sleep(0.3)

            # remove old events
            new_events=[]
            for e in self.events:
                if e['vol']>0.001:
                    new_events.append(e)
            self.events = new_events
            #print(str(len(self.events))+" events...")
