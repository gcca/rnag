%{
#include <math.h>
#include "siete.tab.h"
%}

DGT	[0-9]

%option noyywrap

%%

[ \t]+
{DGT}+"."{DGT}*		{sscanf(yytext+1,"%lf",&yylval.R);return NUM;}
{DGT}+		{sscanf(yytext+1,"%lf",&yylval.R);return NUM;}

%%
