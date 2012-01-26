from django.conf.urls.defaults import patterns, url
from m.views import call_view, SummaryView, TithiView, AspectView, MoonMoveView

def view_url(regexp, view_class):
    return url(regexp, call_view, {'view_class': view_class})

urlpatterns = patterns('',
    view_url(r'^$', SummaryView),
    view_url(r'summary$', SummaryView),
    view_url(r'aspects$', AspectView),
    view_url(r'tithi$', TithiView),
    view_url(r'moon_move$', MoonMoveView),

    url(r'^text/(?P<event_id>\d+)$', 'm.views.event_text'),
)