from django.conf.urls.defaults import patterns, include, url
from django.conf import settings
from django.contrib import admin
admin.autodiscover()

urlpatterns = []

#if settings.DEBUG:
urlpatterns += patterns('',
    (r'^i/(?P<path>.*)$', 'django.views.static.serve', {'document_root': settings.IMAGE_ROOT}),
)

urlpatterns += patterns('',
    url(r'^m/', include('m.urls')),

    url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
    url(r'^admin/', include(admin.site.urls)),
)

