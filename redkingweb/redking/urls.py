from django.conf.urls import patterns, url
from django.conf.urls import include

from redking import views

urlpatterns = patterns('',
    url(r'^$', views.index, name='index'),
    url(r'^sim/(?P<pk>\d+)/$', views.SimView.as_view(), name='sim'),
)
