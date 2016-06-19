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
    # additional..
    cp.pstart = int(math.floor(random.uniform(0,25)))
    cp.hstart = int(math.floor(random.uniform(0,25)))
    cp.model_type = int(math.floor(random.uniform(0,2)))
    return cp

def mutate_cp(cp):
    c = int(random.uniform(0,11))
    if c==0: cp.amin = random.uniform(0,10)
    if c==1: cp.amax = random.uniform(cp.amin,10)
    if c==2: cp.a_p = random.uniform(0,5)+0.1
    if c==3: cp.betmin = random.uniform(0,10)
    if c==4: cp.bemaxtime = random.uniform(cp.betmin,10)
    if c==5: cp.beta_p = random.uniform(0,5)
    if c==6: cp.g = random.uniform(-10,10)
    if c==7: cp.h = random.uniform(-10,10)
    # additional..
    if c==8: cp.pstart = int(math.floor(random.uniform(0,25)))
    if c==9: cp.hstart = int(math.floor(random.uniform(0,25)))
    if c==10: cp.model_type = int(math.floor(random.uniform(0,2)))
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
    print("cp.amin = "+str(cp.amin)+"\n")
    print("cp.amax = "+str(cp.amax)+"\n")
    print("cp.a_p = "+str(cp.a_p)+"\n")
    print("cp.betmin = "+str(cp.betmin)+"\n")
    print("cp.bemaxtime = "+str(cp.bemaxtime)+"\n")
    print("cp.beta_p = "+str(cp.beta_p)+"\n")
    print("cp.g = "+str(cp.g)+"\n")
    print("cp.h = "+str(cp.h)+"\n")

def cp_to_str(cp):
    return("cp.model_type = "+str(cp.model_type)+"\n"+
           "cp.amin = "+str(cp.amin)+"\n"+
           "cp.amax = "+str(cp.amax)+"\n"+
           "cp.a_p = "+str(cp.a_p)+"\n"+
           "cp.betmin = "+str(cp.betmin)+"\n"+
           "cp.bemaxtime = "+str(cp.bemaxtime)+"\n"+
           "cp.beta_p = "+str(cp.beta_p)+"\n"+
           "cp.g = "+str(cp.g)+"\n"+
           "cp.h = "+str(cp.h)+"\n"+
           "cp.pstart = "+str(cp.pstart)+"\n"+
           "cp.hstart = "+str(cp.hstart))

def str_to_cp(s):
    lines = s.split('\n')
    cp = redking.model_cost_params()
    if lines[0].split(' ')[0]=='cp.model_type':
        cp.model_type = int(lines[0].split(' ')[2])
        cp.amin = float(lines[1].split(' ')[2])
        cp.amax = float(lines[2].split(' ')[2])
        cp.a_p = float(lines[3].split(' ')[2])
        cp.betmin = float(lines[4].split(' ')[2])
        cp.bemaxtime = float(lines[5].split(' ')[2])
        cp.beta_p = float(lines[6].split(' ')[2])
        cp.g = float(lines[7].split(' ')[2])
        cp.h = float(lines[8].split(' ')[2])
        # additional..
        cp.pstart = int(lines[9].split(' ')[2])
        cp.hstart = int(lines[10].split(' ')[2])
    elif lines[0].split(' ')[0]=='cp.amin':
        cp.model_type = 1
        cp.amin = float(lines[0].split(' ')[2])
        cp.amax = float(lines[1].split(' ')[2])
        cp.a_p = float(lines[2].split(' ')[2])
        cp.betmin = float(lines[3].split(' ')[2])
        cp.bemaxtime = float(lines[4].split(' ')[2])
        cp.beta_p = float(lines[5].split(' ')[2])
        cp.g = float(lines[6].split(' ')[2])
        cp.h = float(lines[7].split(' ')[2])
        # additional..
        cp.pstart = int(lines[8].split(' ')[2])
        cp.hstart = int(lines[9].split(' ')[2])
    else:
        print(s)
        return False
    return cp

def parasite_state_array(model):
    return [redking.rk_real_getitem(model.get_parasites(),i)
            for i in range(0,model.size())]

def host_state_array(model):
    return [redking.rk_real_getitem(model.get_hosts(),i)
            for i in range(0,model.size())]

def parasite_cost(model):
    return [redking.rk_real_getitem(model.get_parasite_cost(),i)
            for i in range(0,model.size())]

def host_cost(model):
    return [redking.rk_real_getitem(model.get_host_cost(),i)
            for i in range(0,model.size())]

def parasite_matrix(model):
    ret = []
    for i in range(0, model.size()):
        m = model.get_matrix_row(i)
        ret+=[redking.rk_real_getitem(m,j)
              for j in range(model.size())]
    return ret


def safelog(n):
    if n==0:
        return 0;
    else:
        return math.log10(n)

def combined_array(model):
    #return [safelog(redking.rk_real_getitem(m.get_hosts(),i) +
    #                redking.rk_real_getitem(m.get_parasites(),i))
    #        for i in range(0,model.size())]
    return [redking.rk_real_getitem(model.get_hosts(),i) +
            redking.rk_real_getitem(model.get_parasites(),i)
            for i in range(0,model.size())]
