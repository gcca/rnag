#include "gcca_rna.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <malloc.h>
#include <math.h>

struct gcca_rna_t {
	gcca_longitud_t n;
	gcca_longitud_t *ne;
	gcca_real_t **ae;
	gcca_real_t ***pe;
};

static inline gcca_real_t** V2(const gcca_longitud_t I,
											 const gcca_longitud_t J){
	register gcca_longitud_t i;
	gcca_real_t **m=
		(gcca_real_t**)malloc(I*sizeof(gcca_real_t*));
	for(i=0;i<I;i++)
		m[i]=(gcca_real_t*)calloc(J,sizeof(gcca_real_t));
	return m;}

void gcca_rna_inicializar(struct gcca_rna_t *rna,
													gcca_longitud_t n,
													...){
	inline gcca_real_t azar(gcca_real_t a,
													gcca_real_t b)
	{return(b-a)*drand48()+a;}

	va_list args;
	register gcca_longitud_t i,j,k;

	rna->n=n;
	rna->ne=
		(gcca_longitud_t*)malloc(n*sizeof(gcca_longitud_t));
	rna->ae=
		(gcca_real_t**)malloc(n*sizeof(gcca_real_t*));
	rna->pe=
		(gcca_real_t***)malloc((n-1)*sizeof(gcca_real_t**));

	va_start(args,n);
	for(i=0;i<n;i++) rna->ne[i]=va_arg(args,gcca_longitud_t);
	(*rna->ne)++;
	va_end(args);

	for(i=0;i<n;i++){
		rna->ae[i]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));
		for(j=0;j<rna->ne[i];j++)
			rna->ae[i][j]=1.0;}

	for(i=0;i<n-1;i++)
		rna->pe[i]=V2(rna->ne[i],rna->ne[i+1]);

	srand48(time(gcca_NULO));

	for(i=0;i<n-1;i++)
		for(j=0;j<rna->ne[i];j++)
			for(k=0;k<rna->ne[i+1];k++)
				rna->pe[i][j][k]=azar(-.2,.2);

	for(i=0;i<rna->ne[n-2];i++)
		for(j=0;j<rna->ne[n-1];j++)
			rna->pe[n-2][i][j]=azar(-2,2);}

void gcca_rna_duplicar(struct gcca_rna_t *rna_o,
											 struct gcca_rna_t *rna_d){
	register gcca_longitud_t i,j,k;

	rna_d->n=rna_o->n;
	rna_d->ne=
		(gcca_longitud_t*)malloc(rna_o->n*sizeof(gcca_longitud_t));
	rna_d->ae=
		(gcca_real_t**)malloc(rna_o->n*sizeof(gcca_real_t*));
	rna_d->pe=
		(gcca_real_t***)malloc((rna_o->n-1)*sizeof(gcca_real_t**));

	for(i=0;i<rna_o->n;i++)
		rna_d->ne[i]=rna_o->ne[i];

	for(i=0;i<rna_o->n;i++)
		rna_d->ae[i]=(gcca_real_t*)malloc(rna_o->ne[i]*sizeof(gcca_real_t));

	rna_d->ae[0][rna_o->ne[0]-1]=rna_o->ae[0][rna_o->ne[0]-1];

	for(i=0;i<rna_o->n-1;i++)
		rna_d->pe[i]=V2(rna_o->ne[i],rna_o->ne[i+1]);

	for(i=0;i<rna_o->n-1;i++)
		for(j=0;j<rna_o->ne[i];j++)
			for(k=0;k<rna_o->ne[i+1];k++)
				rna_d->pe[i][j][k]=rna_o->pe[i][j][k];}


void gcca_rna_finalizar(struct gcca_rna_t *rna){
	register gcca_longitud_t i,j;

	for(i=0;i<rna->n;i++)
		free(rna->ae[i]);
	free(rna->ae);

	for(i=0;i<rna->n-1;i++){
		for(j=0;j<rna->ne[i];j++)
			free(rna->pe[i][j]);
		free(rna->pe[i]);
	}
	free(rna->pe);

	free(rna->ne);
}

gcca_real_t* gcca_rna_propagar(struct gcca_rna_t *rna,
															 gcca_real_t *e){
	gcca_real_t S(gcca_real_t x){return tanh(x);}
	//gcca_real_t fact(gcca_real_t x){return 1.0/(1.0+exp(-x));}
	register gcca_longitud_t i,j,k;

	for(i=0;i<*rna->ne-1;i++)
		(*rna->ae)[i]=e[i];

	for(i=0;i<rna->n-1;i++)
		for(j=0;j<rna->ne[i+1];j++){
			register gcca_real_t s=0;
			for(k=0;k<rna->ne[i];k++)
				s+=rna->ae[i][k]*rna->pe[i][k][j];
			rna->ae[i+1][j]=S(s);}

	return rna->ae[rna->n-1];}

static inline void retropropagar(struct gcca_rna_t *rna,
																 double *s,
																 double N,double M,double **d,
																 double ***ce){
	double dS(double x){return 1.0-x*x;}
	//double dS(double x){return x*(1.0-x);}
	register gcca_longitud_t i,j,k;

	for(i=0;i<rna->ne[rna->n-1];i++)
		(*d)[i]=dS(rna->ae[rna->n-1][i])*(s[i]-rna->ae[rna->n-1][i]);

	for(i=0;i<rna->n-2;i++)
		for(j=0;j<rna->ne[rna->n-i-2];j++){
			register double e=0;
			for(k=0;k<rna->ne[rna->n-i-1];k++)
				e+=d[i][k]*rna->pe[rna->n-i-2][j][k];
			d[i+1][j]=dS(rna->ae[rna->n-i-2][j])*e;}

	for(i=0;i<rna->n-1;i++)
		for(j=0;j<rna->ne[rna->n-i-2];j++)
			for(k=0;k<rna->ne[rna->n-i-1];k++){
				register double c=d[i][k]*rna->ae[rna->n-i-2][j];
				rna->pe[rna->n-i-2][j][k]+=N*c+M*ce[rna->n-i-2][j][k];
				ce[rna->n-i-2][j][k]=c;}}

void gcca_rna_entrenar(struct gcca_rna_t *rna,
											 gcca_longitud_t n,
											 gcca_real_t **e,
											 gcca_real_t **s){
	register gcca_longitud_t i,j;
	gcca_real_t *d[rna->n-1];

	for(i=rna->n-1,j=0;i;i--,j++)
		d[j]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));
	d[j]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));

	const gcca_longitud_t I=10000;
	const gcca_real_t N=.035;
	const gcca_real_t M=.0005;

	for(i=0;i<I;i++)
		for(j=0;j<n;j++){
			gcca_rna_propagar(rna,e[j]);
			retropropagar(rna,s[j],N,M,d,0);
		}

	for(i=0;i<rna->n;i++)free(d[i]);
}

void gcca_rna_entrenarINM(rna,n,e,s,I,N,M)
		 struct gcca_rna_t *rna;
		 gcca_longitud_t n,I;
		 gcca_real_t **e,**s,N,M;
{
	register gcca_longitud_t i,j;
	gcca_real_t *d[rna->n-1];

	for(i=rna->n-1,j=0;i;i--,j++)
		d[j]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));
	d[j]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));

	I=I?:10000;
	N=N?:.035;
	M=M?:.0005;

	for(i=0;i<I;i++)
		for(j=0;j<n;j++){
			gcca_rna_propagar(rna,e[j]);
			retropropagar(rna,s[j],N,M,d,0);}

	for(i=0;i<rna->n;i++)free(d[i]);}

void gcca_rna_entrenarINMJ(rna,n,e,s,I,N,M,J,K)
		 struct gcca_rna_t *rna;
		 gcca_longitud_t n,I,J,K;
		 gcca_real_t **e,**s,N,M;
{
	static struct gcca_rna_t rna_v[10240];
	static gcca_longitud_t rna_v_n=0;

	register gcca_longitud_t i,j;

	gcca_real_t *d[rna->n-1];
	gcca_real_t ***ce=(gcca_real_t***)malloc((n-1)*sizeof(gcca_real_t**));
	for(i=0;i<rna->n-1;i++)
		ce[i]=V2(rna->ne[i],rna->ne[i+1]);

	struct gcca_rna_t rna_;

	gcca_rna_inicializar(&rna_,rna->n,rna->ne[0]-1,
											 rna->ne[1],rna->ne[2],
											 rna->ne[3],rna->ne[4],rna->ne[5]);

	for(i=rna->n-1,j=0;i;i--,j++)
		d[j]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));
	d[j]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));

	/*I=I?:10000;
	N=N?:.035;
	M=M?:.0005;
	*/
	gcca_real_t err_m=1;

	for(i=0;i<I;i++){
		for(j=0;j<n-J;j++){
			gcca_rna_propagar(&rna_,e[j]);
			retropropagar(&rna_,s[j],N,M,d,ce);
		}
		//F(n,e,s)
		for(j=n-J;j<n;j++){
			register gcca_real_t err=0;
			register gcca_longitud_t l;

			gcca_rna_propagar(&rna_,e[j]);
			
			for(l=0;l<rna->ne[rna->n-1];l++){
				register gcca_real_t ds=s[j][l]-rna_.ae[rna->n-1][l];
				err+=.5*ds*ds;}
			
			if(err<err_m){
				printf("err  %u     % .16lf\n",rna_v_n,err);

				gcca_rna_duplicar(&rna_,&rna_v[rna_v_n++]);

				/*register gcca_longitud_t i,j,k;
				for(i=0;i<rna->n-1;i++)
					for(k=0;k<rna->ne[i+1];k++)
						for(j=0;j<rna->ne[i];j++)
							rna->pe[i][j][k]=rna_.pe[i][j][k];
				*/
				err_m=err;
				}
		}

	}
	//	goto ffffs;
	/**/{
		static gcca_real_t aptp[10240];
		static gcca_real_t aptbuf[10240];

		static gcca_real_t *apt=aptp;
		static gcca_real_t *aptb=aptbuf;

		static struct gcca_rna_t *pobl[10240];
		static struct gcca_rna_t *buf[10240];
		register gcca_longitud_t i,j;
		
		for(i=0;i<rna_v_n;i++){
			pobl[i]=&rna_v[i];
			buf[i]=&rna_v[i];
		}

		static struct gcca_rna_t **p=pobl;
		static struct gcca_rna_t **b=buf;

		double __attribute__((always_inline)) irval(double a,double b){
			return (b-a)*drand48()+a;}
		void ordx(int lo,int hi) {
			int i =lo,j=hi;
			double x=apt[(lo+hi)/2];
			do {
				while(apt[i]<x) i++;
				while(apt[j]>x) j--;
				
				if(i<=j) {
					struct gcca_rna_t *temp;
					gcca_real_t tmp;
					temp=p[i];
					p[i]=p[j];
					p[j]=temp;
					tmp=apt[i];
					apt[i]=apt[j];
					apt[j]=tmp;
					i++;
					j--;
				}
			}while(i<=j);
			if(lo<j) ordx(lo,j);
			if(i<hi) ordx(i,hi);
		}

		srand(time(NULL));
#define IMA K
		for(i=0;i<IMA;i++){

			// calc apt
			register gcca_real_t err,merr=1,nerr=1;;
			register gcca_longitud_t il;

			if(i<0)
				for(il=0;il<rna_v_n;il++){
					err=0;
					for(j=0;j<n;j++){
						register gcca_longitud_t l;
					
						gcca_rna_propagar(&rna_v[il],e[j]);
			
						for(l=0;l<rna->ne[rna->n-1];l++){
							register gcca_real_t ds=s[j][l]-p[il]->ae[rna->n-1][l];
							err+=.5*ds*ds;}
						if(j<n-J) err*=0.91;
					}
					err/=rna_v_n;
					apt[il]=err;
					aptb[il]=err;
				}
			else
				for(il=0;il<rna_v_n;il++){
					err=0;
					for(j=0;j<n;j++){
						register gcca_longitud_t l;
					
						gcca_rna_propagar(&rna_v[il],e[j]);
			
						for(l=0;l<rna->ne[rna->n-1];l++){
							register gcca_real_t ds=s[j][l]-p[il]->ae[rna->n-1][l];
							err+=.5*ds*ds;}
					}
					err/=rna_v_n;
					apt[il]=err;
					aptb[il]=err;
				}


			// ord
			ordx(0,rna_v_n-1);

			//parear
			{
				gcca_longitud_t el=(gcca_longitud_t)(rna_v_n*.75);

				for(j=0;j<el;j++){
					b[j]=p[j];
					aptb[j]=apt[j];
				}
				
				for(j=el;j<rna_v_n;j++){
					gcca_longitud_t p_i=rand()%((gcca_longitud_t)(rna_v_n/2));
					gcca_longitud_t p_j=rand()%((gcca_longitud_t)(rna_v_n/2));
					
					gcca_longitud_t r_i=rand()%(rna->n-2);
					gcca_longitud_t r_j=rand()%(rna->ne[r_i]);
					gcca_longitud_t r_k=rand()%(rna->ne[r_i+1]);

					gcca_real_t hmin,hmax,hdif,halfa,hran;

					if(b[p_i]->pe[r_i][r_j][r_k]>=b[p_j]->pe[r_i][r_j][r_k]){
						hmax=b[p_i]->pe[r_i][r_j][r_k];
						hmin=b[p_j]->pe[r_i][r_j][r_k];

					}
					else{
						hmax=b[p_j]->pe[r_i][r_j][r_k];
						hmin=b[p_i]->pe[r_i][r_j][r_k];
					}
					hdif=hmax-hmin;
					halfa=irval(-0.25,1.25);
					hran=hdif*halfa;
					
					gcca_longitud_t si,sj,sk;
					for(si=0;si<rna->n-1;si++)
						for(sk=0;sk<rna->ne[si+1];sk++)
							for(sj=0;sj<rna->ne[si];sj++)
								b[j]->pe[si][sj][sk]=
									b[rand()%2?p_i:p_j]->pe[si][sj][sk];					
					b[j]->pe[r_i][r_j][r_k]=irval(hmin-hran,hmax+hran);
					/*
					if(0) //rand()%RAND_MAX>1)
						{
						gcca_longitud_t m_i=rand()%(rna->n-2);
						gcca_longitud_t m_j=rand()%(rna->ne[m_i]);
						gcca_longitud_t m_k=rand()%(rna->ne[m_i+1]);

						gcca_real_t nabla(gcca_real_t x){
							static gcca_longitud_t iii=0;
							return x*(1-pow(drand48(),pow(1.0-(double)(iii++)/IMA,100)));
						}
#define rmuM 0.1
#define rmum (-rmuM)					
						printf(")))  %.24lf\n     %.24lf\n",
									 b[j]->pe[m_i][m_j][m_k] ,
									 nabla(b[j]->pe[m_i][m_j][m_k]));


						b[j]->pe[m_i][m_j][m_k]+=rand()%2?
							nabla(b[j]->pe[m_i][m_j][m_k]-rmuM):
							nabla(rmum-b[j]->pe[m_i][m_j][m_k]);

							}*/

				}
					
			}

			gcca_puntero_t ptmtp=p;
			p=b;
			b=ptmtp;
			ptmtp=apt;
			apt=aptb;
			aptb=ptmtp;
		}
		{
			if(K==0) p[0]=&rna_;
			register gcca_longitud_t i,j,k;
			for(i=0;i<rna->n-1;i++)
				for(j=0;j<rna->ne[i];j++)
					for(k=0;k<rna->ne[i+1];k++)
						rna->pe[i][j][k]=p[0]->pe[i][j][k];
		}
	}
	/******/
	for(i=0;i<rna_v_n;i++)
		gcca_rna_finalizar(&rna_v[i]);
 
	/*
	if(!J){
		register gcca_longitud_t i,j,k;

		for(i=0;i<rna->n-1;i++)
			for(k=0;k<rna->ne[i+1];k++)
				for(j=0;j<rna->ne[i];j++)
					rna->pe[i][j][k]=rna_.pe[i][j][k];
	}
	*/

 ffffs:

	for(i=0;i<rna->n-1;i++){
		for(j=0;j<rna->ne[i];j++)
			free(ce[i][j]);
		free(ce[i]);
	}
	free(ce);

	gcca_rna_finalizar(&rna_);

	for(i=0;i<rna->n;i++)free(d[i]);}

gcca_real_t*gcca_rna_entrenarINMf(rna,n,e,s,I,N,M,f)
		 struct gcca_rna_t *rna;
		 gcca_longitud_t n,I,f;
		 gcca_real_t **e,**s,N,M;
{
	register gcca_longitud_t i,j,k;
	gcca_real_t *d[rna->n-1];

	for(i=rna->n-1,j=0;i;i--,j++)
		d[j]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));
	d[j]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));

	I=I?:10000;
	N=N?:.035;
	M=M?:.0005;

	register gcca_real_t *verr=
		f?(gcca_real_t*)malloc((I/f)*sizeof(gcca_real_t)):gcca_NULO;

	k=0;
	//gcca_real_t err=0;
	for(i=0;i<I;i++){
		for(j=0;j<n;j++){
			gcca_rna_propagar(rna,e[j]);
			retropropagar(rna,s[j],N,M,d,0);

			/*if(f&&!(i%f)){
				register gcca_longitud_t l;

				for(l=0;l<rna->ne[rna->n-1];l++){
					register gcca_real_t ds=s[j][l]-rna->ae[rna->n-1][l];
					err+=.5*ds*ds;
				}
			}*/
		}
		if(f&&!(i%f)){
			gcca_real_t err=0;

			for(j=0;j<n;j++){
				register gcca_longitud_t l;

				gcca_rna_propagar(rna,e[j]);

				for(l=0;l<rna->ne[rna->n-1];l++){
					register gcca_real_t ds=s[j][l]-rna->ae[rna->n-1][l];
					err+=.5*ds*ds;
				}}

			verr[k++]=err;
			//err=0;
		}
	}	

	for(i=0;i<rna->n;i++)free(d[i]);

	return verr;}

gcca_real_t* gcca_rna_evaluar(struct gcca_rna_t *rna,
															gcca_longitud_t n,
															gcca_real_t **e){
	register gcca_longitud_t i;
	register gcca_real_t *v=
		(gcca_real_t*)malloc(n*sizeof(gcca_real_t));
	
	for(i=0;i<n;i++){
		gcca_rna_propagar(rna,e[i]);
		v[i]=rna->ae[rna->n-1][0];
	}

	return v;
}

void gcca_rna_imprimir(struct gcca_rna_t *rna,
											 gcca_longitud_t n,
											 gcca_real_t **e){
	register gcca_longitud_t i,j;
	
	for(i=0;i<n;i++){
		gcca_rna_propagar(rna,e[i]);
		printf("( % .4lf",e[i][0]);
		for(j=1;j<rna->ne[0]-1;j++)
			printf(", % .4lf",e[i][j]);
		printf(" ) -> [ % .8lf",rna->ae[rna->n-1][0]);
		for(j=1;j<rna->ne[rna->n-1]-1;j++)
			printf(", % .8lf",rna->ae[rna->n-1][j]);
		printf(" ]\n");}}

void gcca_rna_guardar(struct gcca_rna_t *rna,char *cf){
	register gcca_longitud_t i,j,k;

	FILE *f=fopen(cf,"w");

	fwrite(&rna->n,sizeof(gcca_longitud_t),1,f);

	for(i=0;i<rna->n;i++)
		fwrite(&rna->ne[i],sizeof(gcca_longitud_t),1,f);

	for(i=0;i<rna->n-1;i++)
		for(k=0;k<rna->ne[i+1];k++)
			for(j=0;j<rna->ne[i];j++)
				fwrite(&rna->pe[i][j][k],sizeof(gcca_real_t),1,f);

	fclose(f);}

void gcca_rna_cargar(struct gcca_rna_t *rna,char *cf){
	register gcca_longitud_t i,j,k;

	FILE *f=fopen(cf,"r");

	fread(&rna->n,sizeof(gcca_longitud_t),1,f);

	rna->ne=
		(gcca_longitud_t*)malloc(rna->n*sizeof(gcca_longitud_t));
	rna->ae=
		(gcca_real_t**)malloc(rna->n*sizeof(gcca_real_t*));
	rna->pe=
		(gcca_real_t***)malloc((rna->n-1)*sizeof(gcca_real_t**));

	for(i=0;i<rna->n;i++)
		fread(&rna->ne[i],sizeof(gcca_longitud_t),1,f);

	for(i=0;i<rna->n;i++){
		rna->ae[i]=(gcca_real_t*)malloc(rna->ne[i]*sizeof(gcca_real_t));
		for(j=0;j<rna->ne[i];j++)
			rna->ae[i][j]=1.0;}

	for(i=0;i<rna->n-1;i++)
		rna->pe[i]=V2(rna->ne[i],rna->ne[i+1]);

	for(i=0;i<rna->n-1;i++)
		for(k=0;k<rna->ne[i+1];k++)
			for(j=0;j<rna->ne[i];j++)
				fread(&rna->pe[i][j][k],sizeof(gcca_real_t),1,f);

	fclose(f);}
