from django.conf.urls import patterns, url
from django.conf.urls import include

from redking import views

urlpatterns = patterns('',
    url(r'^$', views.LiveSimListView.as_view(), name='index'),
    url(r'^about', views.about, name='about'),
    url(r'^new', views.live, name='live'),
    url(r'^save_livesim/', views.save_livesim, name='save_livesim'),
    url(r'^livesim/(?P<pk>\d+)/$', views.LiveSimView.as_view())
)
