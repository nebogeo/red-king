from django.shortcuts import render
from django.views import generic
from models import *
import random

def random_colour():
    l = ["#ffe400","#ff8400","#fe647e","#6fb3c8"]
    return l[random.randrange(0,len(l))]

class SimListView(generic.ListView):
    queryset = Sim.objects.order_by('-created_date')
    template_name = 'redking/index.html'
    paginate_by = 10

class SimView(generic.DetailView):
    model = Sim
    template_name = 'redking/sim.html'
