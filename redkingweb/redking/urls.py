from django.conf.urls import patterns, url
from django.conf.urls import include

from redking import views

urlpatterns = patterns('',
    url(r'^$', views.LiveSimListView.as_view(), name='index'),
    url(r'^favorites', views.SimBestView.as_view(), name='index'),
    url(r'^sim/(?P<pk>\d+)/$', views.SimView.as_view()),
    url(r'^up', views.upvote, name='up'),
    url(r'^down', views.downvote, name='down'),
    url(r'^live_index', views.LiveSimListView.as_view(), name='live_index'),
    url(r'^new', views.live, name='live'),
    url(r'^save_livesim/', views.save_livesim, name='save_livesim'),
    url(r'^live_sim/(?P<pk>\d+)/$', views.LiveSimView.as_view())
)
