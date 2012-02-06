from django.conf.urls.defaults import patterns, url

urlpatterns = patterns('',
    url(r'e(?P<event_id>\d+)/$', 'm.views.event_text'),
    url(r'(?P<where>before|after/(?P<event_type>\d+)-(?P<limit_date>\d{12})/$', 'm.views.relative_text'),
    url(r'h/(?P<planet>\d+)-(?P<start>\d{12})-(?P<end>\d{12})/$', 'm.views.hour_text'),
)