from PIL import Image
from PIL import ImageDraw
import numpy as np
from matplotlib import pyplot as plt
import redking
import simulation

class thumb:
    def __init__(self,width,height,compress):
        self.width = width/compress
        self.height = height
        self.x_per_render = 1/float(compress)
        im = Image.new("RGB", (self.width+1,self.height), "black")
        self.pixels=np.array(im)
        self.pos = 0

    def render(self,model):
        for i in range(0,model.size()):
            p = simulation.safelog(redking.rk_real_getitem(model.get_parasites(),i))
            h = simulation.safelog(redking.rk_real_getitem(model.get_hosts(),i))
            c = self.pixels[(model.size()-i)-1,int(self.pos)]
            p=abs(p)*0.1
            h=abs(h)*0.1
            c[0] += (h*254 + p*111)
            c[1] += (h*100 + p*179)
            c[2] += (h*126 + p*200)

            if c[0]>255: c[0]=255
            if c[1]>255: c[1]=255
            if c[2]>255: c[2]=255

            self.pixels[(model.size()-i)-1,int(self.pos)]=c
        self.pos+=self.x_per_render

    def save(self,filename):
        image = Image.fromarray(np.uint8(self.pixels))
        d_usr = ImageDraw.Draw(image)
        print("saving: "+filename)
        image.save(filename)
