from django.conf.urls.defaults import patterns, include, url

urlpatterns = patterns('',
    url(r'citylist/(?P<location_id>\d+)/(?P<year>\d{4})', 'desktop.views.citylist_sunrise'),
    url(r'citylist/(?P<location_id>\d+)', 'desktop.views.citylist_sunrise'),
    url(r'citylist', 'desktop.views.citylist'),

    url(r'dl/(?P<mode>\d)-(?P<country_id>\d+)-(?P<state_id>\d+)-(?P<year>\d{4})', 'desktop.views.dl_view'),
    url(r'dl$', 'desktop.views.dl_view'),

    url(r'geo$', 'desktop.views.geo_view'),

    url(r'login', 'desktop.views.login_view'),
    url(r'logout', 'desktop.views.logout_view'),
    url(r'(?P<page>\w+)', 'desktop.views.main'),
    url(r'$', 'desktop.views.home_redirect'),
)