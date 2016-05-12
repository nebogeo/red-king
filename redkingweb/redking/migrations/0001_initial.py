# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Sim',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('views', models.IntegerField(default=0)),
                ('created_date', models.DateTimeField(verbose_name=b'date created')),
                ('status', models.IntegerField(default=0)),
                ('base_name', models.CharField(max_length=4096)),
                ('params', models.CharField(max_length=4096)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
    ]
