from django.db import models
from datetime import datetime


class Sim(models.Model):
    views = models.IntegerField(default=0)
    created_date = models.DateTimeField('date created')
    status = models.IntegerField(default=0)
    base_name = models.CharField(max_length=4096)
    params = models.CharField(max_length=4096)
    def __unicode__(self):
        return str(self.base_name+"-"+str(self.created_date));
