from django.conf.urls.defaults import patterns, include, url

urlpatterns = patterns('',
    url(r'citylist/(?P<id>\d+)', 'desktop.views.citylist_sunrise'),
    url(r'citylist', 'desktop.views.citylist'),
    url(r'login', 'desktop.views.login_view'),
    url(r'logout', 'desktop.views.logout_view'),
    url(r'(?P<page>\w+)', 'desktop.views.main'),
    url(r'$', 'desktop.views.home_redirect'),
)