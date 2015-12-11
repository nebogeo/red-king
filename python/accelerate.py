import math
import numpy as np
import scipy.io.wavfile
import redking
import copy

def parasite_state_array(model):
    return [redking.rk_real_getitem(m.get_parasites(),i)
            for i in range(0,model.size())]

def host_state_array(model):
    return [redking.rk_real_getitem(m.get_hosts(),i)
            for i in range(0,model.size())]

def safelog(n):
    if n==0:
        return 0;
    else:
        return math.log10(n)

def combined_array(model):
    #return [safelog(redking.rk_real_getitem(m.get_hosts(),i) +
    #                redking.rk_real_getitem(m.get_parasites(),i))
    #        for i in range(0,model.size())]
    return [redking.rk_real_getitem(m.get_hosts(),i) +
            redking.rk_real_getitem(m.get_parasites(),i)
            for i in range(0,model.size())]


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
            ret += math.sin(self.oscs[i])*l
            self.oscs[i]+=self.freq*(i+1)
        self.t+=1
        return ret*0.002

def grain_render(m,s,frames,buf,pos):
    env = 0
    env_size=20
    for i in range(0,frames):
        if i<env_size: env=i/float(env_size)
        elif i>frames-env_size: env=(frames-i)/float(env_size)
        else: env=1
        buf[pos]=s.render()*env
        pos+=1
        #print(out[i])
        if i%10==0:
            s.update(combined_array(m))
            m.step()
    return pos

def path(m,s,filename,grains,frames):
    out = np.zeros(frames*grains,dtype=np.float32)
    pos = 0
    for i in range(0,grains):
        v = i/float(grains)
        m.m_cost_params.vmax=v*20
        m.init()
        print(i)
        pos = grain_render(m,s,frames,out,pos)
        scipy.io.wavfile.write(filename,44100,out)


m = redking.model()
#m.set_model(1)
m.m_pstart=20;
m_m_hstart=25;
m.init()
s = synth(m.size())

path(m,s,"test.wav",500,2000)
