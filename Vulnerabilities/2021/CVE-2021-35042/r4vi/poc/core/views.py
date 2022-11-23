from django.db.models import Count
from django.shortcuts import render

# Create your views here.
from poc.core.models import Things


def demo_view(request):
    qs = (
        Things.objects.prefetch_related("tags")
        .annotate(num_tags=Count("tags"))
        .order_by(request.GET.get("order_by", "name"))
    )
    return render(request, "list.html", {"object_list": qs})
