import math
import numpy as np
import iso226
import copy
import techno

def pitch(note):
    return math.pow(2,(note-69)/12.0)*440

class blip:
    def __init__(self,bands,bar_length):
        self.level = [0 for i in range(0,bands)]
        self.blips = []
        self.events = []
        self.bar_length = int(bar_length*44100)
        self.pos = 0
        self.mode = "TECHNO"

    def update(self,level):
        self.blips=[]
        av = 0
        mx = 0
        for i in range(0,len(level)):
            av+=level[i]
            if level[i]>mx: mx=level[i]
        av/=len(level)
        av/=10
        last = 0
        if mx==0: mx=1
        for i in range(0,len(level)):
            if level[i]>av:
                if last==0:
                    self.blips.append([i,level[i]/mx])
                last=1
            else:
                last=0


    def render(self,out):
        if len(self.blips)>0:
            step = self.bar_length/len(self.blips)
            for i,b in enumerate(self.blips):
                if self.mode=="TECHNO":
                    p = pitch(b[0]+39)*4
                else:
                    p = pitch(b[0]+69)*4

                self.events.append({'pos':i*step,
                                    'freq':p,
                                    'tec':techno.techno(0.3+b[1]*0.8,0.3),
                                    'vol':iso226.iso226(90,p)})
            env = 50
            print(self.blips)
            for i in range(0,self.bar_length):
                if self.pos<len(out):
                    for e in self.events:
                        if self.mode=="TECHNO":
                            s = 0.008*e['tec'].generate(self.pos/44100.0*e['freq'])*e['vol']
                        else:
                            s = 0.008*math.sin(self.pos/44100.0*e['freq'])*e['vol']

                        if i>e['pos'] and i<=e['pos']+env:
                            env_lev = 1-(e['pos']+env-i)/float(env)
                            out[self.pos] += s*env_lev
                        if i>e['pos']+env:
                            out[self.pos] += s
                            e['vol']*=0.999
                    self.pos+=1

            new_events=[]
            for e in self.events:
                if e['vol']>0.001:
                    new_events.append(e)
            self.events = new_events
            #print(str(len(self.events))+" events...")
