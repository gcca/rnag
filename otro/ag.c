#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <limits.h>
#include <float.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <dlfcn.h>
#include <unistd.h>

#define AG_CRMS CN
extern int AG_CRMS;
extern int CRT;

#define MP_LM (20)

#define AG_MUL (1)
#define AG_POBL		(2048*AG_MUL)
//2048
#define AG_IMAX		(512*20)
//65535
//16348
#define AG_RELIT	.1
#define AG_RMUT		.35
//.25f
#define AG_MUT		RAND_MAX*AG_RMUT

struct ag_indv_t {
	double crms[64];
	double apt;
};

//
/*#define AX 1
	#define IN 0
	#define CRT AX*/
/*
	#if CRT
	#define INVOL (-DBL_MAX)
	#else
	#define INVOL DBL_MAX
	#endif
*/
/*inline double F(double *crms) {
	if(
	#include <rr>
	) return INVOL;
	return (
	#include <oo>
	);
	}

	double *X;
*/
/*#if CRT*/
double INVOL;
//double INVOL = (-DBL_MAX);
/*#else
	double INVOL = DBL_MAX;
	#endif
*/


int pv(struct ag_indv_t *pobl[],int b,int t) {
	int i;
	int p;
	double vp;
	struct ag_indv_t *tmp;
	p=b;
	vp=pobl[p]->apt;

	for(i=b+1;i<=t;i++)
		if(pobl[i]->apt<vp) {
			struct ag_indv_t *tmp;
			p++;
			tmp=pobl[i];
			pobl[i]=pobl[p];
			pobl[p]=tmp; }
	tmp=pobl[b];
	pobl[b]=pobl[p];
	pobl[p]=tmp;
	return p;
}
/*void ordx(struct ag_indv_t *pobl[],int lo,int hi) {
	int i =lo,j=hi;
	double x=pobl[(lo+hi)/2]->apt;
	do {
	if(CRT){
	while(pobl[i]->apt>x) i++;
	while(pobl[j]->apt<x) j--;
	}else{
	while(pobl[i]->apt<x) i++;
	while(pobl[j]->apt>x) j--;
	}
	if(i<=j) {
	struct ag_indv_t *temp;
	temp=pobl[i];
	pobl[i]=pobl[j];
	pobl[j]=temp;
	i++;
	j--;
	}
	}while(i<=j);

	{

	if(lo<j) ordx(pobl,lo,j);

	if(i<hi) ordx(pobl,i,hi);
	}
	}
*/
void(*ordx)(struct ag_indv_t*[],int,int);
void ordxm(struct ag_indv_t *pobl[],int lo,int hi) {
	int i =lo,j=hi;
	double x=pobl[(lo+hi)/2]->apt;
	do {
		while(pobl[i]->apt<x) i++;
		while(pobl[j]->apt>x) j--;
		if(i<=j) {
			struct ag_indv_t *temp;
			temp=pobl[i];
			pobl[i]=pobl[j];
			pobl[j]=temp;
			i++;
			j--;
		}
	}while(i<=j);

	{

		if(lo<j) ordx(pobl,lo,j);

		if(i<hi) ordx(pobl,i,hi);
	}
}

//
void impr0(struct ag_indv_t *pobl[]) {
	static long bcrms[6]={0};
	//static double bapt={0};
	register int i,j=0;
	/*
	for(i=0;i<AG_CRMS;i++)
		if(bcrms[i]==(long)(pobl[0]->crms[i]*100))
			j++;

	//	if(j==6) return;

	for(i=0;i<AG_CRMS;i++)
		bcrms[i]=(long)(pobl[0]->crms[i]*100);
	*/
	/*printf("\e[2J\e[2;0H% 8.2lf",pobl[0]->crms[0]);
		for(i=1;i<AG_CRMS;i++)
		printf(" | % 8.2lf",pobl[0]->crms[i]);
		printf("  -> % 8.2lf\n",pobl[0]->apt);*/

	//printf("\e[2J\e[2;0H");
	/*
		int d=AG_CRMS%2==0?(AG_CRMS)/2:(AG_CRMS+1)/2;
		for(i=0;i<AG_CRMS/2-1;i++){
		printf("x[% d] = % 14.2lf\t\tx[% d] = % 14.2lf\n",i,pobl[0]->crms[i],i+d,pobl[0]->crms[i+d]);
		}
		if(i+d>=AG_CRMS+1)
		printf("x[% d] = % 14.2lf\n",i,pobl[0]->crms[i]);
		else
		printf("x[% d] = % 14.2lf\t\tx[% d] = % 14.2lf\n",i,pobl[0]->crms[i],i+d,pobl[0]->crms[i+d]);
	*/
	for(i=0;i<AG_CRMS;i++){
		printf("x[% d] = % 14.4lf\n",i,pobl[0]->crms[i]);
	}
	printf("m%s: %-14.14lf\n",CRT?"ax":"in",pobl[0]->apt);


#define NN 15
	/*static int mm[NN];
		static int mmi=0;
	*/
	//mm[mmi++]=(int)(pobl[0]->apt*1000);

	//if(mmi==NN){
	//	mmi=0;
	//}
	/*	double suus=0;
	for(i=0;i<NN-1;i++){
		double unn=pobl[0]->apt- pobl[i]->apt;
		suus+=unn;
		printf(": %-12.24lf\n",unn);
	}

	printf("-> %-12.24lf\n",suus/(NN-1));
*/
	//		usleep(50);
}

void ordxM(struct ag_indv_t *pobl[],int lo,int hi) {
	int i=lo;
	int j=hi;
	double x=pobl[(lo+hi)/2]->apt;

	do {

		while(pobl[i]->apt>x) i++;
		while(pobl[j]->apt<x) j--;
		if(i<=j) {
			struct ag_indv_t *temp;
			temp=pobl[i];
			pobl[i]=pobl[j];
			pobl[j]=temp;
			i++;
			j--;
		}
	}while(i<=j);

	if(lo<j) ordx(pobl,lo,j);

	if(i<hi) ordx(pobl,i,hi);
}


void elit(struct ag_indv_t *pobl[],struct ag_indv_t *buf[],size_t e) {
	register int i;

	for(i=0;i<e;i++) {
		register int j;
		for(j=0;j<AG_CRMS;j++)
			buf[i]->crms[j]=pobl[i]->crms[j];
		buf[i]->apt=pobl[i]->apt; }
}
inline double irval(double a,double b){return (b-a)*drand48()+a;}

extern double(rf)(double*);
extern double(of)(double*);
void ag_algrtm(void) {
	int i;
	//static struct ag_indv_t indvp[AG_POBL];
	struct ag_indv_t *indvp=
		(struct ag_indv_t*)malloc(AG_POBL*sizeof(struct ag_indv_t));
	//static struct ag_indv_t *pobl[AG_POBL];
	struct ag_indv_t **pobl=
		(struct ag_indv_t**)malloc(AG_POBL*sizeof(struct ag_indv_t*));;

	//static struct ag_indv_t indvb[AG_POBL];
	struct ag_indv_t *indvb=
		(struct ag_indv_t*)malloc(AG_POBL*sizeof(struct ag_indv_t));;
	//static struct ag_indv_t *buf[AG_POBL];
	struct ag_indv_t **buf=
		(struct ag_indv_t**)malloc(AG_POBL*sizeof(struct ag_indv_t*));;

	struct ag_indv_t **p=pobl;
	struct ag_indv_t **b=buf;

	for(i=0;i<AG_POBL;i++) {
		pobl[i]=&indvp[i];
		buf[i]=&indvb[i];

		int j;
		for(j=0;j<AG_CRMS;j++){
			pobl[i]->crms[j]=irval(-500.2,500.2);
		}
	}

	for(i=0;i<AG_IMAX;i++) {
		{
			register int i;
			for(i=0;i<AG_POBL;i++){
				p[i]->apt=rf(p[i]->crms)?of(p[i]->crms):INVOL;
			}}{
			ordx(p,0,AG_POBL-1);
			//impr0(p,i);
			double s=0;
			register int j;
			for(j=1;j<AG_POBL/2+1;j++){
				//double u=pobl[0]->apt- pobl[j]->apt;
				double u=pobl[j]->apt;
				s+=u/(AG_POBL/2);}
			s=CRT?pobl[0]->apt-s:s-pobl[0]->apt;
			//printf("%-12.14lf\n",s);
			if(s<0.000000001&&i>2048)break;

		}{
			register int ii;
			size_t e=AG_POBL*AG_RELIT;
			{
				elit(p,b,e);
				for(ii=e;ii<AG_POBL;ii++) {
					int x=rand()%(AG_POBL/2);
					int y=rand()%(AG_POBL/2);
					int z=rand()%AG_CRMS;
					double hm,hM,dh,ha,hR;
					if(b[x]->crms[z]>=b[y]->crms[z]){
						hM=b[x]->crms[z];
						hm=b[y]->crms[z];}
					else{
						hM=b[y]->crms[z];
						hm=b[x]->crms[z];
					}
					dh=hM-hm;
					ha=irval(-0.25,1.25);
					hR=dh*ha;

					b[ii]->crms[z]=irval(hm-hR,hM+hR);
					register int j;
					for(j=0;j<z;j++)
						b[ii]->crms[j]=b[rand()%2?x:y]->crms[j];
					for(j++;j<AG_CRMS;j++)
						b[ii]->crms[j]=b[rand()%2?x:y]->crms[j];

					int im=rand()%AG_CRMS;
					double l=b[ii]->crms[im]*0.2;
					b[ii]->crms[im]+=
						(rand()%2?l-b[ii]->crms[im]:-l-b[ii]->crms[im])*
						(1-pow(drand48(),pow(1-i/AG_POBL,0.5)));
				}
			}}{
			register struct ag_indv_t **tmp;
			tmp=p;
			p=b;
			b=tmp;
		}}
	impr0(p);
	free(indvp);
	free(indvb);
	free(pobl);
	free(buf);
	printf("\n");
}
unsigned long long gettimestamp() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec * 1000000 + t.tv_usec;
}

void ss()
{
	//printf("\e[0;37m");
	exit(0);
}

int main() {
	srand48(time(NULL));
	srand(time(NULL));
	signal(SIGINT,ss);

	long long e1,e2;

	//void *mr;
	//register char *err;

	//	printf("\e[2J\e[1;37m");


	/*
		if(!(mr=dlopen("./feo.so", RTLD_LAZY))){
		fprintf(stderr,"%s\n",dlerror());
		exit(1);}

		rf=dlsym(mr,"rf");
		of=dlsym(mr,"of");
		if((err=dlerror())!=NULL){
		fprintf(stderr,"%s\n",err);
		exit(1);}
		AG_CRMS=*(int*)dlsym(mr,"N");
		if((err=dlerror())!=NULL){
		fprintf(stderr,"%s\n",err);
		exit(1);}
		CRT=*(int*)dlsym(mr,"M");
		if((err=dlerror())!=NULL){
		fprintf(stderr,"%s\n",err);
		exit(1);}*/
	/*
		INVOL=CRT?-DBL_MAX:DBL_MAX;
		ordx=CRT?ordxM:ordxm;
	*/
	if(CRT){
		INVOL=-DBL_MAX;
		ordx=ordxM;}
	else{
		INVOL=DBL_MAX;
		ordx=ordxm;}

	//e1 = gettimestamp();

	ag_algrtm();

	//e2 = gettimestamp();


	//printf("\e[0;37m");
	//dlclose(mr);
	//printf("\n\t%lld\n%lld\n\n\t%lf\n",e2-e1,LLONG_MIN,(double)DBL_MAX);
	//printf("\t%lld\n",e2-e1);
	return 0;
}
