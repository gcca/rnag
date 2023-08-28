from django.conf.urls.defaults import patterns, include, url

# from django.contrib import admin
# admin.autodiscover()

urlpatterns = patterns('',
	url(r'^rna/$','rna.vista.rna'),
  url(r'^rna/ini/','rna.vista.ini'),
	url(r'^ag/','rna.vista.ag'),
# url(r'^$', 'rna.views.home', name='home'),
# url(r'^rna/', include('rna.foo.urls')),

# url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
# url(r'^admin/', include(admin.site.urls)),
)
