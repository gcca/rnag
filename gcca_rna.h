#ifndef GCCA_RNA_
#define GCCA_RNA_

#include "gcca_defs.h"

#define gcca_rna_def(rna)															\
	char _##rna[80];																		\
	struct gcca_rna_t *rna=(struct gcca_rna_t*)_##rna;

#define gcca_rna_vector_ (gcca_real_t[])

struct gcca_rna_t;

void gcca_rna_inicializar(struct gcca_rna_t*,
													gcca_longitud_t,...);
void gcca_rna_finalizar(struct gcca_rna_t*);

gcca_real_t* gcca_rna_propagar(struct gcca_rna_t*,
															 gcca_real_t*);

void gcca_rna_entrenar(struct gcca_rna_t*,
											 gcca_longitud_t,
											 gcca_real_t**,gcca_real_t**);
void gcca_rna_entrenarINM(struct gcca_rna_t*,
													gcca_longitud_t,
													gcca_real_t**,gcca_real_t**,
													gcca_longitud_t,gcca_real_t,gcca_real_t);
void gcca_rna_entrenarINMJ(struct gcca_rna_t*,
													 gcca_longitud_t,
													 gcca_real_t**,gcca_real_t**,
													 gcca_longitud_t,gcca_real_t,gcca_real_t,
													 gcca_longitud_t,gcca_longitud_t);
gcca_real_t* gcca_rna_entrenarINMf(struct gcca_rna_t*,
																	 gcca_longitud_t,
																	 gcca_real_t**,gcca_real_t**,
																	 gcca_longitud_t,gcca_real_t,gcca_real_t,
																	 gcca_longitud_t);

gcca_real_t* gcca_rna_evaluar(struct gcca_rna_t*,
															gcca_longitud_t,
															gcca_real_t**);

void gcca_rna_imprimir(struct gcca_rna_t*,
											 gcca_longitud_t,
											 gcca_real_t**);

void gcca_rna_guardar(struct gcca_rna_t*,char*);
void gcca_rna_cargar(struct gcca_rna_t*,char*);

#endif
