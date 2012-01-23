from django.conf.urls.defaults import patterns, url

urlpatterns = patterns('',
    url(r'summary$', 'm.views.summary'),
    url(r'aspects$', 'm.views.aspect_list'),
    url(r'tithi$', 'm.views.tithi_list'),
    url(r'moon_move$', 'm.views.moon_move_list'),

    url(r'^text/(?P<event_id>\d+)$', 'm.views.event_text'),
)