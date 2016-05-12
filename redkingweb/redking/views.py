from django.shortcuts import render
from django.views import generic
from models import *

def index(request):
    context = {}
    context['sims'] = Sim.objects.all().order_by('-created_date')
    return render(request, 'redking/index.html', context)

class SimView(generic.DetailView):
    model = Sim
    template_name = 'redking/sim.html'
