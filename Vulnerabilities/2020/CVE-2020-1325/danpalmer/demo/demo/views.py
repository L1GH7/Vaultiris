from django.shortcuts import redirect, render
from django.core.cache import cache


def view_get(request):
    if 'k' in request.GET:
        results = cache.get(request.GET['k'])
        return render(request, 'index.html', {'results': results})
    return render(request, 'index.html')


def view_set(request):
    cache.set(request.GET['k'], request.GET['v'])
    return redirect(view_get)
