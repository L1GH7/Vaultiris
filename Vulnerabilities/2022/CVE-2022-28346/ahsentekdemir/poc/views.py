from django.db.models import Count
from django.http import HttpResponse

from .models import Blog


def blogs(request):
    field = request.GET.get('field', 'title')
    blogs_count = Blog.objects.annotate(**{field: Count("title")})

    data = ""
    for blog_count in blogs_count:
        data += f"<h2>{blog_count.title}</h2>"
    return HttpResponse(data)