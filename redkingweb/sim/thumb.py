from PIL import Image
from PIL import ImageDraw
import numpy as np
from matplotlib import pyplot as plt
import redking

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
            p = redking.rk_real_getitem(model.get_parasites(),i)
            h = redking.rk_real_getitem(model.get_hosts(),i)
            c = self.pixels[(model.size()-i)-1,int(self.pos)]
            c[0] += h*255
            c[1] += p*255
            self.pixels[(model.size()-i)-1,int(self.pos)]=c
        self.pos+=self.x_per_render

    def save(self,filename):
        image = Image.fromarray(np.uint8(self.pixels))
        d_usr = ImageDraw.Draw(image)
        print("saving: "+filename)
        image.save(filename)
