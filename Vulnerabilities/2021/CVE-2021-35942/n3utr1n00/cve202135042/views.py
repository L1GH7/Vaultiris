from django.http import HttpResponse
from .models import Wolf

def loadexampledata(request):
    html=""
    u=Wolf(name="Night-Wolf")
    u.save()
    u=Wolf(name="White-Wolf")
    u.save()
    u=Wolf(name="Three Wolves")
    u.save()
    return HttpResponse(html)

def wolves(request):
    # wolves = Wolf.objects.order_by(request.GET.get('order_by', default='id'))
    a = {"SUMMARY true) SELECT 1; --": True}
    wolves = Wolf.objects.explain(**a)
    print(wolves)
    # rows = ""
    # for wolf in wolves:
    #     rows += f"<tr><td>{wolf.id}</td><td>{wolf.name}</td></tr>" 
    # table = "<table><tbody>%s</tbody></table>" %rows
    # html = "<html><center><body><h1>Wolves</h1>%s</body></center></html>" %table
    # return HttpResponse(html)
