import math
import numpy as np
import iso226
import copy

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
        self.blips=[]
        av = 0
        for i in range(0,len(level)):
            av+=level[i]
        av/=len(level)
        av/=10
        last = 0
        for i in range(0,len(level)):
            if level[i]>av:
                if last==0:
                    self.blips.append(i)
                last=1
            else:
                last=0



    def render(self,out):
        if len(self.blips)>0:
            step = self.bar_length/len(self.blips)
            for i,b in enumerate(self.blips):
                p = pitch(b+69)
                self.events.append({'pos':i*step,
                                    'freq':p,
                                    'vol':iso226.iso226(1,p)})
            env = 100
            print(self.blips)
            for i in range(0,self.bar_length):
                if self.pos<len(out):
                    for e in self.events:
                        s = 0.02*math.sin(self.pos/44100.0*e['freq'])*e['vol']
                        if i>e['pos'] and i<=e['pos']+env:
                            env_lev = 1-(e['pos']+env-i)/float(env)
                            out[self.pos] += s*env_lev
                        if i>e['pos']+env:
                            out[self.pos] += s
                            e['vol']*=0.9993
                    self.pos+=1

            new_events=[]
            for e in self.events:
                if e['vol']>0.001:
                    new_events.append(e)
            self.events = new_events
            #print(str(len(self.events))+" events...")
