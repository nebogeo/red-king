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

from django.db import models
from datetime import datetime

class Sim(models.Model):
    views = models.IntegerField(default=0)
    created_date = models.DateTimeField('date created')
    status = models.IntegerField(default=0)
    base_name = models.CharField(max_length=4096)
    params = models.CharField(max_length=4096)
    length = models.FloatField(default=0)
    upvotes = models.IntegerField(default=0)
    downvotes = models.IntegerField(default=0)
    twitter_likes = models.IntegerField(default=0)
    twitter_retweets = models.IntegerField(default=0)
    fitness = models.FloatField(default=0)

    def __unicode__(self):
        return str(self.base_name+"-"+str(self.created_date));
