import math
import numpy as np
import iso226
import copy

class synth:
    def __init__(self,bands):
        self.freq = 0.01
        self.oscs = [0 for i in range(0,bands)]
        self.level = [0 for i in range(0,bands)]
        self.last_level = [0 for i in range(0,bands)]
        self.t = 0
        self.t_max = 10
        self.loudness_curve = iso226.iso226(1,[i*20 for i in range(1,bands+1)])
        self.loudness_curve = map(lambda i: i*0.1, self.loudness_curve)

    def update(self,level):
        self.last_level=copy.copy(self.level)
        self.level = copy.copy(level)
        # clamp
        for i in range(0,len(self.level)):
            self.level[i]*=self.loudness_curve[i]
            if self.level[i]>1: self.level[i]=1
            if self.level[i]<0: self.level[i]=0
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
