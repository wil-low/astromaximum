from django.conf.urls.defaults import patterns, url
from m.views import call_view_login_required, TextView

def view_url_login_required(regexp, view_class):
    return url(regexp, call_view_login_required, {'view_class': view_class})

urlpatterns = patterns('',
    view_url_login_required(r'(?P<direction>[eba])(?P<event_id>\d+)/$', TextView),
)