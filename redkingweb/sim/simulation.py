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
    cp.amin = 0
    cp.amax = 10
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

def cp_to_str(cp):
    return("amin:"+str(cp.amin)+", "+
           "amax:"+str(cp.amax)+", "+
           "a_p:"+str(cp.a_p)+", "+
           "betmin:"+str(cp.betmin)+", "+
           "betmax:"+str(cp.bemaxtime)+", "+
           "beta_p:"+str(cp.beta_p)+", "+
           "g:"+str(cp.g)+", "+
           "h:"+str(cp.h))

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
