#include <Python.h>

#include "../gcca_rna.h"

gcca_longitud_t n0;

gcca_rna_def(rna_i);

static PyObject*
gcca_rna(PyObject *pyo,PyObject *args){
	PyObject *cs;
	PyObject *es;
	PyObject *ss;
	PyObject *iter;
	PyObject *elm;
	gcca_longitud_t ep;
	gcca_longitud_t mag;
	gcca_longitud_t eex;


	static gcca_longitud_t ne[10];
	static gcca_real_t e[512];
	static gcca_real_t s[512];
	static gcca_real_t *ev[128];
	static gcca_real_t *sv[128];
	gcca_longitud_t n;

	register gcca_longitud_t i,I;

	if(!PyArg_ParseTuple(args,"OOOIII",&cs,&es,&ss,&ep,&mag,&eex))
		return NULL;

	i=0;
	if(!(iter=PyObject_GetIter(cs)))
		return NULL;
	while(elm=PyIter_Next(iter)){
		ne[i++]=PyLong_AsLong(elm);
		Py_DECREF(elm);
	}
	Py_DECREF(iter);
	n=i;
	n0=ne[0];

	gcca_rna_inicializar(rna_i,i,ne[0],ne[1],ne[2],ne[3],ne[4],ne[5]);

	i=0;
	if(!(iter=PyObject_GetIter(es)))
		return NULL;
	while(elm=PyIter_Next(iter)){
		e[i++]=PyFloat_AsDouble(elm);
		Py_DECREF(elm);
	}
	Py_DECREF(iter);

	i=0;
	if(!(iter=PyObject_GetIter(ss)))
		return NULL;
	while(elm=PyIter_Next(iter)){
		s[i++]=PyFloat_AsDouble(elm);
		Py_DECREF(elm);
	}
	Py_DECREF(iter);
	I=i;

	for(i=0;i<I;i++){
		ev[i]=&e[i*(*ne)];
		sv[i]=&s[i*ne[n-1]];}

	gcca_rna_entrenarINMJ(rna_i, I, ev, sv, ep,0.035,0.0005,mag,eex);
	/*
	for(i=0;i<I;i++){
		gcca_longitud_t j;
		for(j=0;j<*ne;j++)
			printf("%.2lf  ",ev[i][j]);
		printf("\n");
		}*/
	
	
	gcca_real_t *verr=gcca_rna_evaluar(rna_i, I, ev);
	/*
	for(i=0;i<I;i++)
		printf("%u  %lf\n",I,verr[i]);
	printf("\n");
	*/
	PyObject *tupla=PyTuple_New(I);

	for(i=0;i<I;i++){
		PyTuple_SetItem(tupla,i,
										PyFloat_FromDouble(verr[i]));
	}

	free(verr);

	return tupla;
}

static PyObject*
gcca_evaluar(PyObject *pyo,PyObject *args){
	PyObject *dv;
	PyObject *iter;
	register PyObject *elm;

	if(!PyArg_ParseTuple(args,"O",&dv))
		return NULL;
	
	static gcca_real_t e[512];
	static gcca_real_t *ev[128];

	gcca_longitud_t n;
	register gcca_longitud_t i;

	i=0;
	if(!(iter=PyObject_GetIter(dv)))
		return NULL;
	while(elm=PyIter_Next(iter)){
		e[i++]=PyFloat_AsDouble(elm);
		Py_DECREF(elm);
	}
	Py_DECREF(iter);
	n=i/n0;

	for(i=0;i<n;i++)
		ev[i]=&e[i*n0];

 	gcca_real_t *sv=gcca_rna_evaluar(rna_i,n,ev);

	PyObject *tupla=PyTuple_New(n);

	for(i=0;i<n;i++){
		PyTuple_SetItem(tupla,i,
										PyFloat_FromDouble(sv[i]));
	}

	free(sv);

	return tupla;
	
}


static PyMethodDef ci_metodos[]={
	{"gcca_rna",gcca_rna,METH_VARARGS,"gcca_rna"},
	{"gcca_evaluar",gcca_evaluar,METH_VARARGS,"gcca_evaluar"},
	{NULL}};

PyMODINIT_FUNC
initci(void){
	PyObject *m;

	if(!(m=Py_InitModule("ci",ci_metodos)))
		return;
}
