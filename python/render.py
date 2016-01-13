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
import scipy.io.wavfile
import redking
import copy
import random
from simulation import *
from PIL import Image
from PIL import ImageDraw
from matplotlib import pyplot as plt

def image_render(m,frames,pos,y,pixels):
    for i in range(0,frames):
        p = parasite_state_array(m)
        h = host_state_array(m)
        for x in range(0,m.size()):
            pc = p[x]*1000
            if pc>255: pc=255
            if pc<0: pc=0
            hc = h[x]*500
            if hc>255: hc=255
            if hc<0: hc=0
            pixels[i+y,pos+x]=(pc,0,hc)
        m.step()
        m.step()
        m.step()
        m.step()

def plot_graph(data,x,y,h,pixels):
    dmin = 999999
    dmax = 0
    for e in data:
        if e<dmin: dmin=e
        if e>dmax: dmax=e
    for i,e in enumerate(data):
        pixels[y+(e-dmin)/(dmax-dmin)*h,x+i]=(255,255,255)

def matrix_render(data,size,slices,x,y,h,pixels):
    span = size/slices
    dmin = 999999
    dmax = 0
    for e in data:
        if e<dmin: dmin=e
        if e>dmax: dmax=e

    for s in range(0,slices-1):
        start = s*size*span
        for i,e in enumerate(data[start:start+size]):
            pixels[y+(e-dmin)/(dmax-dmin)*h,x+i]=(255,255,255)

def curve_render(m,xpos,ypos,pixels):
    # host tradeoff
    plot_graph(parasite_cost(m),xpos,ypos,20,pixels)
    plot_graph(host_cost(m),xpos,ypos+21,20,pixels)
    matrix_render(parasite_matrix(m),m.size(),6,xpos,ypos+42,20,pixels)

def blendpath(m,a,b,filename,grains,frames):
    im = Image.new("RGB", (grains*(m.size()+1),(frames/4)+80), "black")
    pixels=np.array(im)
    pos = 0
    for i in range(0,grains):
        v = i/float(grains)
        m.m_cost_params=blend_cp(a,b,v)
        m.init()
        print(i,pos)
        curve_render(m,pos,0,pixels)
        image_render(m,frames/4,pos,73,pixels)
        pos = pos+m.size()+1

        image = Image.fromarray(np.uint8(pixels))
        #d_usr = ImageDraw.Draw(image)
        #d_usr = d_usr.text((0,h-10),qname,(100,100,100))

        print("saving: "+filename)
        image.save(filename)



m = redking.model()


# copied from sonification
def big_seed(i):
    random.seed(i)
    a = random_cp()
    b = random_cp()
    print("----------------- "+str(i))
    print_cp(a)
    print("->")
    print_cp(b)
    print("")
    blendpath(m,a,b,"random_path-"+str(i)+"-0-big.png",100,600)


big_seed(21)
big_seed(23)
big_seed(38)
big_seed(62)
