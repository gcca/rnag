from django.shortcuts import render_to_response
from django import forms

import ci

from pylab import *

class FormRNA(forms.Form):
	def proc(self):
		return []

class FormRNA_DesdeWeb(FormRNA):
	entradas = forms.CharField(widget=forms.Textarea)
	salidas = forms.CharField(widget=forms.Textarea)
	capas = forms.CharField(max_length=100)

	def proc(self):
		return ci.gcca_rna(map(int,self['capas'].value().split(' ')),
											 map(float,self['entradas'].value().split('\r\n')),
											 map(float,self['salidas'].value().split('\r\n')))

class FormRNA_DesdeFichero(FormRNA):
	fichero = forms.FileField()

#{% for d in resultado %}
#{{ d }} <br>
#{% endfor %}

def inicio(peticion):
	if peticion.method == 'POST':
		form = FormRNA_DesdeFichero(peticion.POST,peticion.FILES)
		#print map(float,form['entradas'].value().split('\r\n'))
		#ddd = form.proc()
		print peticion.FILES
		if form.is_valid():
			print peticion.FILES
			
			ddd=[2,4,6,8]
			fig = figure()
			a=fig.add_subplot(111)
			a.plot(ddd)
			fig.savefig('/dev/shm/uno.png')
		
	else:
		form = FormRNA_DesdeFichero()

	return render_to_response('rna.html', {'form': form, 'resultado':('dd')})
