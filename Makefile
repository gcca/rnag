FUENTE	= gcca_rna.c gcca_rna_demo.c
OBJ   	= ${FUENTE:.c=.o}

all: rna_demo gcca_rna

.c.o:
	@echo cc $<
	@gcc -Wall -I. -c $<

rna_demo: ${OBJ}
	@echo cc -o $@
	@gcc -o $@ ${OBJ} -lm

gcca_rna: gcca_rna.o
	ar -r libgcca_rna.a gcca_rna.o

limpiar:
	@rm ${OBJ} rna_demo libgcca_rna.a