from django.conf.urls.defaults import patterns, include, url

# from django.contrib import admin
# admin.autodiscover()

import vista

urlpatterns = patterns('',
	url(r'^rna/$',vista.inicio)
    # url(r'^$', 'rna.views.home', name='home'),
    # url(r'^rna/', include('rna.foo.urls')),

    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
    # url(r'^admin/', include(admin.site.urls)),
)
