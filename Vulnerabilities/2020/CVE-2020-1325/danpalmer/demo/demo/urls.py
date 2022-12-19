from django.urls import path

from . import views

urlpatterns = [
    path('', views.view_get),
    path('set', views.view_set),
]
