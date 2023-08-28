from django.shortcuts import render_to_response
from django import forms

from django.template import Context, RequestContext

from django.http import HttpResponseRedirect
from subprocess import call
import ci
import os


from pylab import *

inid = {
	'entradas' : """
0.0833333, 0.565944, 0, 0, 1, 0.742222, 0, 0, 1,
0.166667, 0.618576, 0, 0, 1, 0.672, 0, 0, 1,
0.25, 0.567183, 0.5, 0.285714, 1, 0.709333, 0.333333, 0.166667, 0,
0.333333, 0.59195, 1, 0.714286, 1, 0.688889, 0.333333, 0.333333, 0,
0.416667, 0.663777, 0, 0, 1, 0.715556, 0.333333, 0.166667, 1,
0.5, 0.652632, 1, 0.428571, 0, 0.721778, 0.333333, 0.666667, 0,
0.583333, 0.608669, 1, 0.571429, 0, 0.709333, 0, 0, 1,
0.666667, 0.600619, 0.5, 0.428571, 1, 0.731556, 0.333333, 0.5, 1,
0.75, 0.60805, 0, 0, 1, 0.652444, 0, 0, 1,
0.833333, 0.594427, 0, 0, 0, 0.652444, 0.333333, 0.666667, 0,
0.916667, 0.601858, 0.5, 0.571429, 1, 0.642667, 0.666667, 0.833333, 1,
1, 0.593808, 0, 0, 1, 0.681778, 0, 0, 1,
0.0833333, 0.611765, 0.5, 0.571429, 1, 0.706667, 0, 0, 0,
0.166667, 0.619814, 0, 0, 0, 0.682667, 0.666667, 0.5, 1,
0.25, 0.618576, 0, 0, 1, 0.710222, 0.333333, 0.333333, 1,
0.333333, 0.610526, 0, 0, 1, 0.717333, 0, 0, 1,
0.416667, 0.673065, 0.5, 1, 1, 0.734222, 0, 0, 1,
0.5, 0.70774, 1, 0.285714, 0, 0.725333, 0.666667, 0.166667, 1,
0.583333, 0.733127, 0.5, 0.285714, 1, 0.827556, 0, 0, 1,
0.666667, 0.73065, 1, 0.857143, 1, 0.752889, 0, 0, 1,
0.75, 0.747988, 0, 0, 0, 0.824889, 0.666667, 0.5, 1,
0.833333, 0.778328, 0.5, 0.142857, 1, 0.775111, 0.333333, 0.5, 1,
0.916667, 0.747368, 0.5, 0.714286, 0, 0.792, 0, 0, 1,
1, 0.771517, 0, 0, 1, 0.786667, 0.333333, 0.333333, 1,
0.0833333, 0.767183, 0, 0, 1, 0.845333, 0, 0, 1,
0.166667, 0.769659, 0.5, 0.428571, 0, 0.687111, 0.666667, 0.333333, 1,
0.25, 0.767183, 1, 0.142857, 0, 0.722667, 1, 0.833333, 0,
0.333333, 0.762848, 0.5, 0.142857, 0, 0.792889, 0.666667, 0.5, 1,
0.416667, 0.774613, 0.5, 0.142857, 1, 0.813333, 0, 0, 0,
0.5, 0.771517, 0, 0, 1, 0.736889, 0.333333, 0.166667, 0,
0.583333, 0.763467, 1, 0.285714, 1, 0.795556, 0, 0, 1,
0.666667, 0.757895, 0.5, 0.142857, 0, 0.656889, 0, 0, 1,
0.75, 0.757276, 0, 0, 1, 0.679111, 0, 0, 0,
0.833333, 0.750464, 0, 0, 1, 0.669333, 0, 0, 1,
0.916667, 0.770898, 0, 0, 1, 0.695111, 0.333333, 0.5, 1,
1, 0.760372, 0.5, 0.142857, 0, 0.678222, 0, 0, 1,
0.0833333, 0.827864, 0.5, 0.285714, 0, 0.702222, 0.666667, 0.666667, 0,
0.166667, 0.883591, 0, 0, 1, 0.721778, 0.333333, 0.333333, 1,
0.25, 0.84582, 0.5, 0.428571, 0, 0.643556, 0.666667, 1, 1,
0.333333, 0.89226, 1, 0.142857, 1, 0.813333, 0.666667, 0.833333, 0,
0.416667, 0.896594, 1, 0.857143, 1, 1, 0, 0, 1,
0.5, 0.895356, 0, 0, 1, 0.807111, 0.333333, 0.5, 0,
0.583333, 0.962848, 0, 0, 0, 0.832889, 0.666667, 0.666667, 0,
0.666667, 1, 0, 0, 1, 0.849778, 0.333333, 0.666667, 1,
0.75, 0.910217, 0.5, 0.142857, 0, 0.84, 0.333333, 0.5, 0,

0.833333, 0.882353, 0.5, 0.285714, 1, 0.799111, 0.333333, 0.166667, 1,
0.916667, 0.895356, 0.5, 0.571429, 1, 0.778667, 0, 0, 1,
1, 0.818576, 0, 0, 0, 0.777778, 0.666667, 1, 1,
0.0833333, 0.952941, 0.5, 0.428571, 1, 0.802667, 0.666667, 0.666667, 0,
0.166667, 0.793808, 0, 0, 1, 0.740444, 0.333333, 0.666667, 1,
0.25, 0.875542, 0.5, 0.142857, 0, 0.788444, 0.333333, 0.666667, 0,
	""",
	'salidas':"""
0.521,
0.37313,
0.398666,
0.763564,
0.518897,
0.548098,
0.649823,
0.624827,
0.592742,
0.46458,
0.59887,
0.370546,
0.721204,
0.249414,
0.321216,
0.375233,
0.914859,
0.689479,
0.711711,
1.0,
0.174308,
0.484828,
0.703118,
0.325723,
0.508742,
0.427988,
0.374692,
0.191372,
0.477258,
0.463919,
0.599591,
0.571532,
0.524305,
0.50033,
0.377276,
0.495223,
0.473232,
0.383885,
0.125578,
0.459052,
0.709728,
0.286487,
0.198101,
0.294959,
0.362375,

0.555429,
0.586252,
0.349516,
0.460314,
0.407078,
0.339242,
	""",
	'consulta':"""
0.0833333, 0.767683, 0, 0, 1, 0.845333, 0, 0, 1,
0.166667, 0.769679, 0.5, 0.428571, 0, 0.687111, 0.666667, 0.333333, 0,
0.25, 0.767183, 1, 0.142857, 0, 0.722667, 1, 0.833333, 0,
0.333333, 0.762828, 0.5, 0.142857, 0, 0.792889, 0.666667, 0.5, 1,
0.416667, 0.774613, 0.5, 0.142857, 1, 0.813333, 0, 0, 0,
0.5, 0.771511, 0, 0, 1, 0.736889, 0.333333, 0.166667, 0,
0.583333, 0.743467, 1, 0.285714, 1, 0.795556, 0, 0, 1,
0.666667, 0.757895, 0.5, 0.142857, 0, 0.656889, 0, 0, 1,
0.75, 0.757276, 0, 0, 1, 0.679111, 0, 0, 0,
0.833333, 0.750464, 0, 0, 1, 0.669333, 0, 0, 1,
0.916667, 0.776898, 0, 0, 1, 0.695111, 0.333333, 0.5, 1,
1, 0.760372, 0.5, 0.142857, 0, 0.678222, 0, 0, 1,
	""",
	'numero_entradas':'9',
	'datos_prueba': '40',
	'epocas':'5000',
	}


#{% for d in resultado %}
#{{ d }} <br>
#{% endfor %}

def maneja(f):
	for chunk in f.chunks():
		print chunk

class FormINI(forms.Form):
	RNA_OPC = (
		('F', 'Formato'),
		('L', 'Lineal'),
		('H', 'Fichero'),
    )
	rna = forms.ChoiceField(choices=RNA_OPC)

class FormRNA_Formato(forms.Form):
	formato = forms.CharField(widget=forms.Textarea)
#	capas = forms.CharField(max_length=100)
	def proc(self):
		pass

from django.core.exceptions import ValidationError
#import ast
import parser

def valtupla(v):
	try:
		parser.expr('['+v+']')
	except Exception, e:
		raise ValidationError('linea: '+str(e.args[1][1])+
													' -- caracter: '+str(e.args[1][2])+
													' -- texto: '+e.args[1][3])

class FormRNA_Lineal(forms.Form):
	entradas = forms.CharField(widget=forms.Textarea,
														 validators=[valtupla])
	salidas = forms.CharField(widget=forms.Textarea,
														validators=[valtupla])
	consulta = forms.CharField(widget=forms.Textarea,
														 validators=[valtupla],
														 required=False)
	numero_entradas = forms.IntegerField()
	datos_prueba  = forms.IntegerField()
	epocas = forms.IntegerField()

	def proc(self):
		ee = eval('[' + self['entradas'].value() + ']')
		ss = eval('[' + self['salidas'].value() + ']')
		cs = eval('[' + self['consulta'].value() + ']')

		n = int(self['numero_entradas'].value())
		m = int(self['datos_prueba'].value())
		ep = int(self['epocas'].value())

		fig = figure(figsize=(9,9))

		z = fig.add_subplot(111)

		rr = []
		re = True
		if len(ss) == len(ee) / n:
			rr = ss[:]

			for i in range(m):
				ss[len(ss) - 1 - i] = ss[len(ss) - m - 1]
			tt=ss
		elif len(ss) < len(ee) / n:
			m = (len(ee) / n) - len(ss)
			rr = ss + [ss[-1]] * m
			tt = ss
			ss = rr
			re = False

		else:
			print 'ERROR'

		if re:
			z.plot(rr,lw=0.5)

		v = ci.gcca_rna([n, 9, 7, 1],ee,ss,ep,m)

		#tt = tuple(map(lambda x: 16643 * x, tt))
		#v = tuple(map(lambda x: 16643 * x, v))

		#z.plot(v,label='RN',lw=6)

		if re:
			z.plot(tt[:len(tt)-m],lw=1.5)
			if m == 0:
				z.plot(v,lw=2)
			else:
				z.plot((None,)*(len(v)-m-1)+(tt[len(v)-m],)+v[len(v)-m:],lw=2)
		else:
			#z.plot(tt+[.0]*m,label='Datos',lw=1.5)
			z.plot(tt,lw=1.5)
			z.plot((None,)*(len(v)-m-1)+(tt[-1],)+v[len(v)-m:],lw=2)


		if cs != []:
			ol = ci.gcca_evaluar(cs)
			om = (None,)*(len(tt)-1)+(v[-1],)+ol
			z.plot(om,lw=2)
			z.axis((0,len(om)-1,0,1.1))
			z.legend((r'Real',r'Datos',r'Pronostico',r'Consulta'),shadow=True)
		else:
			z.legend((r'Real',r'Datos',r'Pronostico'),shadow=True)
			z.axis((0,len(rr)-1,0,1.1))

		for i in z.get_legend().get_texts():
			setp(i, fontsize=8)


#plot([len(s)-m-1]*2,(min(min(s),min(v)),max(max(s),max(v))))
		fig.savefig('/dev/shm/htdocs/media/uno.png')

		del z
		del fig

		return True


#		return ci.gcca_rna(map(int,self['capas'].value().split(' ')),
#											 map(float,self['entradas'].value().split('\r\n')),
#											 map(float,self['salidas'].value().split('\r\n')))

class FormRNA_Fichero(forms.Form):
	fichero = forms.FileField()

	def proc(self):
		pass

def rnahtml(form,c):
	respuesta = render_to_response('rna.html',
																 {'form':form},
																 )
	respuesta.set_cookie('I',c)
	return respuesta

def procerr(v):
	fig = figure()
	z = fig.add_subplot(111)
	z.plot(v)
	z.set_ylabel('error')
	z.set_xlabel('epoca')
	fig.savefig('/dev/shm/htdocs/media/uno.png')
	del z
	del fig

def procform(form):
	resultado = None

	if form.is_valid():
		resultado = form.proc()

		#procerr(resultado)

	return render_to_response(
		'rna.html',
		{'form': form,
		 'resultado': resultado})


def rna(peticion):
	if peticion.COOKIES.has_key('I'):
		if peticion.COOKIES['I'] == 'F':
			return procform(FormRNA_Formato(peticion.POST))

		elif peticion.COOKIES['I'] == 'L':
			return procform(FormRNA_Lineal(peticion.POST))

		elif peticion.COOKIES['I'] == 'H':
			print peticion.FILES

			form = FormRNA_Fichero(peticion.POST,peticion.FILES)

			if form.is_valid():
				pass

			return render_to_response(
				'rna.html',
				{'form': form})

	else:
		form = FormINI(peticion.POST)

		if form.is_valid():
			if form['rna'].value() == 'F':
				return rnahtml(FormRNA_Formato(),'F')

			elif form['rna'].value() == 'L':
				return rnahtml(FormRNA_Lineal(initial=inid),'L')

			elif form['rna'].value() == 'H':
				return rnahtml(FormRNA_Fichero(),'H')

	return render_to_response(
		'inicio.html',
		{'form': FormINI(peticion.POST)})


def ini(peticion):
	respuesta = HttpResponseRedirect('/gcca/rna/')
	respuesta.delete_cookie('I')
	return respuesta


html_ag="""
<html>
	<body>
		<form action=\"/gcca/rna/\" method=\"post\">
			{{ form.as_p }}
			<input type=\"submit\" value=\"Evaluar\" />
		</form>

		<form action=\"/gcca/rna/ini/\" method=\"post\">
			<input type=\"submit\" value=\"X\" />
		</form>

		{% if resultado %}
		<img src=\"/media/uno.png\" style=\"height:120%;width:100%\" />
		{% endif %}

	</body>
</html>

"""

innn={
	'entrada':u'''max:x[0] + x[1] + x[2] + x[3] - x[4] + x[5]
x[0] >0
x[1] > 0
x[2] > 0
x[3] > 0
x[4] > 0
x[5] > 0
sqrt(x[1]) < 4
sqrt(x[2] + x[1]) <= 4
x[3] < 100
x[0] < 100
x[5] < 100
4 * pow(x[3],3) + 2 * pow(x[2], 2) + x[1] < 100	'''
	}

class FormAG(forms.Form):
	entrada = forms.CharField(widget=forms.Textarea)

def ag(p):
	if p.method == 'POST':
		form = FormAG(p.POST)
		if form.is_valid():
			with open('/dev/shm/feo','wb') as f:
				f.write(form['entrada'].value().replace('\r\n','\n'))
			call(['/dev/shm/htdocs/otro','/dev/shm/feo'])
			return render_to_response('ag.html',{'form':form,'resultado':True})

	else:
		form = FormAG(initial=innn)

	return render_to_response('ag.html',{'form':form})
