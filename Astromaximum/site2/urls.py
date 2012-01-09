from django.conf.urls.defaults import patterns, include, url
from django.conf import settings
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    url(r'^$', 'summary.views.main'),

    url(r'^common/gen/(?P<year>\d{4})/$', 'amax.views.common_generate'),
    url(r'^common/show/$', 'amax.views.common_show'),
    url(r'^common/show/(?P<year>\d{4})-(?P<month>\d{2})-(?P<day>\d{2})/$', 'amax.views.common_show'),
    url(r'^common/clean/(?P<year>\d{4})/$', 'amax.views.common_clean'),

    url(r'^gen/(?P<year>\d{4})/(?P<region>\w+)/(?P<city_id>\w{8})/$', 'amax.views.location_generate'),

    url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
    url(r'^admin/', include(admin.site.urls)),
)

if settings.DEBUG:
    urlpatterns += patterns('',
        (r'^i/(?P<path>.*)$', 'django.views.static.serve', {'document_root': settings.IMAGE_ROOT}),
    )
