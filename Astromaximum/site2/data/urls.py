from django.conf.urls.defaults import patterns, url

urlpatterns = patterns('',
    url(r'^$', 'data.views.upload_datafile'),
)