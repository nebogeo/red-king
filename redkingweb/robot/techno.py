import math

class techno:
    def __init__(self,cutoff,resonance):
        self.s = [0,0,0]
        self.f = cutoff
        self.q = resonance

    # resonant low pass filter
    def lpf_modf(self, src):
        fb=self.q+self.q*(1-self.f)
        self.s[0]+=self.f*(src-self.s[0])+fb*(self.s[0]-self.s[1])
        self.s[1]+=self.f*(self.s[0]-self.s[1])
        return self.s[1]

    def bpf_modf(self, src):
        bp=self.s[0]-self.s[1]
        fb=self.q+self.q*(1-self.f)
        self.s[0]+=self.f*(src-self.s[0]+fb*bp)
        self.s[1]+=self.f*(self.s[0]-self.s[1])
        return bp


    def saw(self,i):
        return math.fmod(i/(2*math.pi),1)*2-1

    def generate(self,i):
        return self.bpf_modf(self.saw(i))
