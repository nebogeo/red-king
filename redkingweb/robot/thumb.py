from PIL import Image
from PIL import ImageDraw
import numpy as np
from matplotlib import pyplot as plt
import redking
import sim_helpers

class thumb:
    def __init__(self,width,height,compress):
        self.width = width/compress
        self.height = height
        self.x_per_render = 1/float(compress)
        im = Image.new("RGB", (self.width+1,self.height), "black")
        self.pixels=np.array(im)
        self.pos = 0
        self.parasite_col = [255/255.0,132/255.0,0/255.0]
        self.host_col = [111/255.0,179/255.0,200/255.0]


    def render(self,model):
        for i in range(0,model.size()):
            p = sim_helpers.safelog(redking.rk_real_getitem(model.get_parasites(),i))
            h = sim_helpers.safelog(redking.rk_real_getitem(model.get_hosts(),i))
            c = self.pixels[(model.size()-i)-1,int(self.pos)]

            # squash the log10
            p=min(1,abs(p))
            h=min(1,abs(h))

            # screen blend mode
            col = [(1-self.parasite_col[0]*p)*(1-self.host_col[0]*h),
                   (1-self.parasite_col[1]*p)*(1-self.host_col[1]*h),
                   (1-self.parasite_col[2]*p)*(1-self.host_col[2]*h)]

            # convert to 8 bit
            c[0] = (1-col[0])*255
            c[1] = (1-col[1])*255
            c[2] = (1-col[2])*255

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
