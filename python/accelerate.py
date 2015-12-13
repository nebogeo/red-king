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
        buf[pos]=s.render()*env
        pos+=1
        #print(out[i])
        if i%step==0:
            t = combined_array(m)
            #diff = compare(t,last)
            #av_change = (av_change+diff)/2.0
            #if diff>max_change: max_change=diff
            #print(i,av_change,max_change)
            s.update(t)
            last = t
            m.step()
    return pos

def path(m,s,filename,grains,frames,step):
    out = np.zeros(frames*grains,dtype=np.float32)
    pos = 0
    for i in range(0,grains):
        v = i/float(grains)+0.8
        #m.m_cost_params.g=(v-0.1)*16.0
        #m.m_cost_params.beta_p=0.8+(1-v)
        print(m.m_cost_params.beta_p)
        m.init()
        print(i,m.m_cost_params.g)
        pos = grain_render(m,s,frames,out,pos,step)
        scipy.io.wavfile.write(filename,44100,out)


m = redking.model()
# m.set_model(1)
# m.m_cost_params.amin = 0
# m.m_cost_params.amax = 5.4
# m.m_cost_params.a_p = 2.4
# m.m_cost_params.betamin = 0
# m.m_cost_params.bemaxtime = 9.3
# m.m_cost_params.beta_p = 2.9
# m.m_cost_params.g = -0.2
# m.m_cost_params.h = 0.4

print m.m_cost_params.beta_p

m.m_cost_params.beta_p=0.4

m.init()
s = synth(m.size())

#path(m,s,"test4.wav",500,2000,10)

path(m,s,"single-run.wav",1,500000,100)

#path(m,s,"test.wav",50,25000,20)
