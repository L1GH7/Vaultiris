import os

from django.shortcuts import HttpResponse, redirect


def redirect_test(request):
    page = request.GET.get('page', None)
    if page is not None:
        redirect(page)
    return HttpResponse("OK")


def show_tmp(request):
    return HttpResponse(os.popen('ls /tmp').read())
