from operator import mod
from pyexpat import model
from tracemalloc import start
from django.db import models

class Experiment(models.Model):
    start_datetime = models.DateTimeField()
    start_date = models.DateField(null=True, blank=True)
    start_time = models.TimeField(null=True, blank=True)
    end_datetime = models.DateTimeField(null=True, blank=True)
    end_date = models.DateField(null=True, blank=True)
    end_time = models.TimeField(null=True, blank=True)

class Flag(models.Model):
    secret=models.CharField(max_length=255,unique=True)

    def __str__(self): 
        return self.secret
