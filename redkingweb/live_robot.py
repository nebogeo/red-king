#!/usr/bin/env python
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

import os
import math
import time
import hashlib
import random
import robot.twitter

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "redkingweb.settings")

import django
import datetime
from redking.models import *
from django.utils import timezone
from django.db.models import Max, Count, Sum
from binascii import a2b_base64
from PIL import Image

django.setup()

def save_imgdata(data,filename):
    #print(data[22:])
    fd = open(filename, 'wb')
    fd.write(a2b_base64(data[22:]))
    fd.close()

def append_images(a,b,filename):
    images = map(Image.open, [a, b])
    widths, heights = zip(*(i.size for i in images))

    max_width = max(widths)
    total_height = sum(heights)

    new_im = Image.new('RGB', (max_width, total_height))

    y_offset = 0
    for im in images:
        new_im.paste(im, (0,y_offset))
        y_offset += im.size[1]

    new_im.save(filename)
    

def run(location):
    sims = LiveSim.objects.filter(status=0)
    for sim in sims:
        print sim.id
        host = str(sim.id)+"_host.png"
        parasite = str(sim.id)+"_parasite.png"
        save_imgdata(sim.host_img_data,host)
        save_imgdata(sim.parasite_img_data,parasite)
        final_image=str(sim.id)+"_image.png"
        append_images(host,parasite,final_image)
        os.remove(host)
        os.remove(parasite)
        robot.twitter.tweet("A citizen scientist just created this host/parasite co-evolution music: http://redking.fo.am/sim/"+str(d.id),imgname,robot.twitter.api)
        os.remove(final_image)
        sim.status=1
        sim.save()

'''    if th!=False:
        wavname = location+base_name+".wav"
        imgname = location+base_name+".png"
        scipy.io.wavfile.write(wavname,44100,out)
        os.system("lame "+wavname)
        #os.system("aplay "+wavname)
        os.system("rm "+wavname)
        th.save(imgname)
        os.system("mogrify -resize 600% -filter Point "+imgname)
        d=Sim(created_date = timezone.now(),
              base_name = base_name,
              length = length,
              params = params_str)
        if parent!=False:
            d.parent = parent
        d.save()
'''        
        #robot.twitter.tweet("I just generated new host/parasite evolution music: http://redking.fo.am/sim/"+str(d.id),imgname,robot.twitter.api)

#while(True):
run("media/sim/")
