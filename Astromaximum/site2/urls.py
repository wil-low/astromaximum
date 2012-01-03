from django.conf.urls.defaults import patterns, include, url
from django.conf import settings
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'amax_dj.views.home', name='home'),
    url(r'^$', 'summary.views.main'),

    url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
    url(r'^admin/', include(admin.site.urls)),
)

if settings.DEBUG:
    urlpatterns += patterns('',
        (r'^i/(?P<path>.*)$', 'django.views.static.serve', {'document_root': settings.IMAGE_ROOT}),
    )
