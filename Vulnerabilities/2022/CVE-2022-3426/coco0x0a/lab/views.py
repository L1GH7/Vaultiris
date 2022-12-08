import imp
from django.http.response import JsonResponse
from datetime import datetime
from django.db.models.functions import Extract, Trunc
from django.db.models import DateTimeField
from lab.models import Experiment
from django.core import serializers
from django.shortcuts import render
from django.http import HttpResponse

def vuln_extract(request):
    payload = request.GET.get('lookup_name')
    start = datetime(2015, 6, 15)
    end = datetime(2015, 7, 2)
    Experiment.objects.create(
        start_datetime=start, start_date=start.date(),
        end_datetime=end, end_date=end.date())
    experiments = Experiment.objects.filter(start_datetime__year=Extract('end_datetime', payload))
    return JsonResponse({"res": serializers.serialize("json", experiments)})

def index(request):
    return HttpResponse('<h1>lab CVE-2022-34265</h1>')