import math
import numpy as np
import scipy.io.wavfile
import redking
import copy
import random

# setting up and blending cost parameters
def random_cp():
    cp = redking.model_cost_params()
    cp.amin = random.uniform(0,10)
    cp.amax = random.uniform(cp.amin,10)
    cp.a_p = random.uniform(0,5)+0.1
    cp.betmin = random.uniform(0,10)
    cp.bemaxtime = random.uniform(cp.betmin,10)
    cp.beta_p = random.uniform(0,5)
    cp.g = random.uniform(-10,10)
    cp.h = random.uniform(-10,10)
    return cp

def lerp(a,b,t):
    return a*(1-t)+b*t

def blend_cp(a,b,t):
    cp = redking.model_cost_params()
    cp.amin = lerp(a.amin,b.amin,t)
    cp.amax = lerp(a.amax,b.amax,t)
    cp.a_p = lerp(a.a_p,b.a_p,t)
    cp.betmin = lerp(a.betmin,b.betmin,t)
    cp.bemaxtime = lerp(a.bemaxtime,b.bemaxtime,t)
    cp.beta_p = lerp(a.beta_p,b.beta_p,t)
    cp.g = lerp(a.g,b.g,t)
    cp.h = lerp(a.h,b.h,t)
    return cp

def print_cp(cp):
    print("amin:"+str(cp.amin))
    print("amax:"+str(cp.amax))
    print("a_p:"+str(cp.a_p))
    print("betmin:"+str(cp.betmin))
    print("betmax:"+str(cp.bemaxtime))
    print("beta_p:"+str(cp.beta_p))
    print("g:"+str(cp.g))
    print("h:"+str(cp.h))

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
