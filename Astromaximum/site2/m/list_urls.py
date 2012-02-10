from django.conf.urls.defaults import include, patterns, url
from m.views import call_view, SummaryView, TithiView, AspectView, MoonMoveView, RiseSetView,\
    PlanetHourView, RetrogradeView, SettingsView

def view_url(regexp, view_class):
    return url(regexp, call_view, {'view_class': view_class})

urlpatterns = patterns('',
    view_url(r'summary/$', SummaryView),
    view_url(r'aspects/$', AspectView),
    view_url(r'tithi/$', TithiView),
    view_url(r'moon_move/$', MoonMoveView),
    view_url(r'rise_set(?P<direction>[0-2])/$', RiseSetView),
    view_url(r'hour/$', PlanetHourView),
    view_url(r'retro/$', RetrogradeView),
    view_url(r'settings/$', SettingsView),

    url(r'text/', include('m.text_urls')),
)