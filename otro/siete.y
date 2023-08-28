%{
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

struct simb
{
	char *nom;
	int t;
	union {
		double var;
		double (*func)();
	}val;
	struct simb *sig;
};
typedef struct simb simb;

extern simb *simb_tbl;

simb *inssimb();
simb *obtsimb();
%}

%union {
	double R;
	simb *SIMB;
}

%token <R> NUM
%token <SIMB> VAR FUNC
%type <R> expr

%right '='
%left '-' '+'
%left '*' '/'
%left NEG
%right '^'

%%

en:
	| en lin
;
lin: '\n'
	| expr '\n'		{printf("\t%.10g\n",$1);}
	| error	'\n'	{yyerrok;}
;
expr: NUM
	| VAR 					{$$=$1->val.var}
	| VAR '=' expr			{$$=$3;$1->val.var=$3;}
	| FUNC '(' expr ')'		{}
	| expr '+' expr			{$$=$1+$3;}
	| expr '-' expr			{$$=$1-$3;}
	| expr '*' expr			{$$=$1*$3;}
	| expr '/' expr			{$$=$1/$3;}
	| '-' expr %prec NEG	{$$=-$2;}
	| expr '^' expr			{$$=pow($1,$3);}
	| '(' expr ')'			{$$=$2;}
;

%%

main()
{
	inic_tbl();
	yyparse();
}

yyerror(e)
	char *e;
{
	fprintf(stderr,"%s\n",e);
}

struct inic
{
	char *nom;
	double (*func)();
};

struct inic func_arit[]=
{
	"sin",sin,
	"cos",cos,
	0,0
};

simb *simb_tbl=(simb*)0;

inic_tbl()
{
	register int i;
	simb *ap;
	for(i=0;func_arit[i].nom!=0;i++)
		{
			ap=inssimb(func_arit[i].nom,FUNC);
			ap->val.func=func_arit[i].func;
		}
}

simb*
inssimb(ap_nom,ap_t)
	char *ap_nom;
	int ap_t;
{
	simb *ap=(simb*)malloc(sizeof(simb));
	ap->nom=(char*)malloc(strlen(ap_nom)+1);
	strcpy(ap->nom,ap_nom);
	ap->t=ap_t;
	ap->val.var=0;
	ap->sig=(struct simb*)simb_tbl;
	simb_tbl=ap;
	return ap;
}

simb*
obtsimb(ap_nom)
	char * ap_nom;
{
	register simb *ap;
	for(ap=simb_tbl;ap!=(simb*)0;
			ap=(simb*)ap->sig)
		if(strcmp(ap->nom,ap_nom)==0)
			return ap;
	return 0;
}

/*yylex()
{
	register int c;

	while((c=getchar())==' '||c=='\t');

	if(c==EOF) return 0;

	if(c=='.'||isdigit(c))
		{
			ungetc(c,stdin);
			scanf("%lf",&yylval.R);
			return NUM;
		}

	if(isalpha(c))
		{
			simb *s;
			static char *simbuf=0;
			static int lon=0;
			register int i;

			if(lon==0) lon=40,simbuf=(char*)malloc(lon+1);

			i=0;
			do
				{
					if(i==lon)
						{
							lon*=2;
							simbuf=(char*)realloc(simbuf,lon+1);
						}

					simbuf[i++]=c;

					c=getchar();
				}
			while(c!=EOF&&isalnum(c));

			ungetc(c,stdin);
			simbuf[i]='\0';

			s=obtsimb(simbuf);
			if(s==0) s=inssimb(simbuf,VAR);
			yylval.SIMB=s;
			return s->t;
		}

	return c;
}*/
