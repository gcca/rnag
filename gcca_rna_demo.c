#include <stdio.h>
#include <malloc.h>

#include "gcca_rna.h"

int main(){
	gcca_real_t *e[16] = {
		gcca_rna_vector_{0, 0, 0, 0},
		gcca_rna_vector_{0, 0, 0, 1},
		gcca_rna_vector_{0, 0, 1, 0},
		gcca_rna_vector_{0, 0, 1, 1},

		gcca_rna_vector_{0, 1, 0, 0},
		gcca_rna_vector_{0, 1, 0, 1},
		gcca_rna_vector_{0, 1, 1, 0},
		gcca_rna_vector_{0, 1, 1, 1},

		gcca_rna_vector_{1, 0, 0, 0},
		gcca_rna_vector_{1, 0, 0, 1},
		gcca_rna_vector_{1, 0, 1, 0},
		gcca_rna_vector_{1, 0, 1, 1},

		gcca_rna_vector_{1, 1, 0, 0},
		gcca_rna_vector_{1, 1, 0, 1},
		gcca_rna_vector_{1, 1, 1, 0},
		gcca_rna_vector_{1, 1, 1, 1},
	};

	gcca_real_t *s[16] = {
		gcca_rna_vector_{0.00},
		gcca_rna_vector_{0.08},
		gcca_rna_vector_{0.01},
		gcca_rna_vector_{0.09},

		gcca_rna_vector_{0.01},
		gcca_rna_vector_{0.22},
		gcca_rna_vector_{0.02},
		gcca_rna_vector_{0.07},

		gcca_rna_vector_{0.02},
		gcca_rna_vector_{0.09},
		gcca_rna_vector_{0.19},
		gcca_rna_vector_{0.11},

		gcca_rna_vector_{0.22},
		gcca_rna_vector_{0.13},
		gcca_rna_vector_{0.21},
		gcca_rna_vector_{0.15},
	};

	gcca_rna_def(rna_i);

	gcca_rna_inicializar(rna_i, 4, 4, 7, 6, 1);

	gcca_rna_entrenarINMJ(rna_i, 16, e, s, 50000, 0.035, 0.0005, 4, 0);

	gcca_real_t *v=gcca_rna_evaluar(rna_i,16,e); 

	gcca_rna_imprimir(rna_i, 16, e);

	//gcca_rna_guardar(rna_i, "a.rna");

	//return 0;
	/*
	gcca_rna_cargar(rna_i, "a.rna");

	gcca_rna_imprimir(rna_i, 4, e);
*/
	gcca_longitud_t i;
	for(i=0;i<16;i++)
		printf("  %lf\n",v[i]);

	gcca_rna_finalizar(rna_i);

	return 0;
}
