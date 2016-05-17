import math
import numpy as np
import iso226
import copy

class blip:
    def __init__(self,bands,bar_length):
        self.base_freq = 4
        self.level = [0 for i in range(0,bands)]
        self.t = 0
        self.t_max = 10
        self.loudness_curve = iso226.iso226(1,[i*20 for i in range(1,bands+1)])
        self.loudness_curve = map(lambda i: i*0.1, self.loudness_curve)
        self.blips = []
        self.bar_length = int(bar_length*44100)
        self.pos = 0

    def update(self,level):
        self.level = copy.copy(level)
        self.analyse()

    def analyse(self):
        self.blips=[]
        av = 0
        for i in range(0,len(self.level)):
            av+=self.level[i]
        av/=len(self.level)
        av/=10
        last = 0
        for i in range(0,len(self.level)):
            if self.level[i]>av:
                if last==0:
                    self.blips.append(i)
                last=1
            else:
                last=0


    def render(self,out):
        if len(self.blips)>0:
            events = []
            step = self.bar_length/len(self.blips)
            print(self.blips)
            for i,b in enumerate(self.blips):
                events.append({'pos':i*step,'freq':b,'vol':self.loudness_curve[b]})
            env = 200
            for i in range(0,self.bar_length):
                if self.pos<len(out):
                    for e in events:
                        if i>e['pos'] and i<=e['pos']+env:
                            env_lev = 1-(e['pos']+env-i)/float(env)
                            print env_lev
                            f = (e['freq']*0.03)+0.01
                            out[self.pos] += 0.01*math.sin(self.pos*f)*e['vol']*env_lev
                        if i>e['pos']+env:
                            f = (e['freq']*0.03)+0.01
                            out[self.pos] += 0.01*math.sin(self.pos*f)*e['vol']
                            e['vol']*=0.9995
                    self.pos+=1
