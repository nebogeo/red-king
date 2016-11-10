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

from django.shortcuts import *
from django.views import generic
from django import forms
from django.forms import ModelForm
from models import *
import random

def random_colour():
    l = ["#ffe400","#ff8400","#fe647e","#6fb3c8"]
    return l[random.randrange(0,len(l))]

class SimListView(generic.ListView):
    queryset = Sim.objects.order_by('-created_date')
    template_name = 'redking/index.html'
    paginate_by = 10
    def get_context_data(self, **kwargs):
        context = super(SimListView, self).get_context_data(**kwargs)
        for s in context['object_list']:
            s.score = s.upvotes-s.downvotes
        return context

class SimBestView(generic.ListView):
    queryset = Sim.objects.order_by('-fitness')
    template_name = 'redking/index.html'
    paginate_by = 10
    def get_context_data(self, **kwargs):
        context = super(SimBestView, self).get_context_data(**kwargs)
        for s in context['object_list']:
            s.score = s.upvotes-s.downvotes
        return context

class SimView(generic.DetailView):
    model = Sim
    template_name = 'redking/sim.html'
    def get_context_data(self, **kwargs):
        context = super(SimView, self).get_context_data(**kwargs)
        s = context['sim']
        context['score']=s.upvotes-s.downvotes
        return context

def upvote(request):
    sim = get_object_or_404(Sim, pk=int(request.POST.get('id')))
    sim.upvotes+=1
    sim.save()
    num_votes=sim.upvotes-sim.downvotes
    return HttpResponse(num_votes)

def downvote(request):
    sim = get_object_or_404(Sim, pk=int(request.POST.get('id')))
    sim.downvotes+=1
    sim.save()
    num_votes=sim.upvotes-sim.downvotes
    return HttpResponse(num_votes)

def live(request):
    context={}
    return render(request, 'redking/live.html', context)

class LiveSimListView(generic.ListView):
    queryset = LiveSim.objects.order_by('-created_date')
    template_name = 'redking/live_index.html'
    paginate_by = 10
    def get_context_data(self, **kwargs):
        context = super(LiveSimListView, self).get_context_data(**kwargs)
        return context

class LiveSimForm(ModelForm):
     class Meta:
         model = LiveSim
         fields = "__all__"

## incoming from javascript...
def save_livesim(request):
    if request.method == 'POST':
        form = LiveSimForm(request.POST)    
        if form.is_valid():
            form.save()
            return HttpResponse('')
        else:
            print("invalid form")
    return HttpResponse('request is invalid: '+str(form))
