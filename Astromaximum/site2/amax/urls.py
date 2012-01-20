from django.conf.urls.defaults import patterns, url

urlpatterns = patterns('',
    url(r'^$', 'm.views.summary'),
    url(r'^(?P<year>\d{4})-(?P<month>\d{2})-(?P<day>\d{2})/summary$', 'm.views.summary'),
    url(r'^today/summary$', 'm.views.today_summary'),
    
    url(r'^gen/(?P<year>\d{4})/$', 'amax.views.common_generate'),
    url(r'^show/$', 'amax.views.common_show'),
    url(r'^show/(?P<year>\d{4})-(?P<month>\d{2})-(?P<day>\d{2})/$', 'amax.views.common_show'),
    url(r'^clean/(?P<year>\d{4})/$', 'amax.views.common_clean'),

    url(r'^gen/(?P<year>\d{4})/(?P<region>\w+)/(?P<city_id>\w{8})/$', 'amax.views.location_generate'),
)