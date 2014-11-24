%{

#include "genpacket.tab.h"

%}

%option outfile="genpacket.lex.c"
%option noyywrap

%%

"fixed"             { return PK_TFIXED; }
"dynamic"           { return PK_TDYNAMIC; }
"calculated"        { return PK_TCALC; }
"packet"            { return PACK;}
"{"                 { return PSTART; }
"}"                 { return PEND; }

"frame"             { return PA_FRAME; }
"attr"              { return PA_ATTR; }
"attribute"         { return PA_ATTR; }

"type"              { return OP_TYPE; }
"="                 { return ASSIGN; }

"bit"               { yylval.number = 1; return TYPE; }
"uint8"             { yylval.number = 8; return TYPE; }
"uint16"            { yylval.number = 16; return TYPE; }
"int[0-9].[0-9]?"   { return TYPE; }
"uint[0-9].[0-9]?"  { return TYPE; }

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.string = strdup(yytext); return VAR; }


"#".*  { return COMMENT; }
[0-9]+  { yylval.number = atoi(yytext); return NUMBER; }
\n      { /* printf("\n"); return EOL; */ }
[ \t]   /* return nada */
.       { printf("Unkown: %c\n", *yytext); }
