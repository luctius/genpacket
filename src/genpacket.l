%{

#include <stdlib.h>

#include "packet.h"
#include "genpacket.tab.h"

int line_num = 1;
%}

%option outfile="genpacket.lex.c"
%option noyywrap

%x COMMENTS
%x COMMENTS_LINE
%x PP

%%

"fixed "         { return PK_TFIXED; }
"dynamic "       { return PK_TDYNAMIC; }
"calculated "    { return PK_TCALC; }
"packet "        { BEGIN(PP); return PACKET;}
<PP>"size"       { return PA_SIZE; }
<PP>"pipe"       { return PA_PIPE; }

"frame"          { return PO_FRAME; }
"attr"           { return PO_ATTR; }
"attribute"      { return PO_ATTR; }
"size"           { return PO_SIZE; }
"crc"            { return PO_CRC; }
"data"           { return PO_DATA; }

"type"           { return OP_TYPE; }
"data_width"     { return OP_DATAWIDTH; }
"datawidth"      { return OP_DATAWIDTH; }
"data_size"      { return OP_DATASIZE; }
"datasize"       { return OP_DATASIZE; }
"default"        { return OP_DEFAULT; }
"values"         { return OP_VALUES; }
"exclude"        { return OP_EXCLUDE; }
"start"          { return OP_START; }
"end"            { return OP_END; }

<INITIAL,PP>"="  { return '='; }
<INITIAL,PP>"{"  { BEGIN(INITIAL); return '{'; }
<INITIAL,PP>"}"  { BEGIN(INITIAL); return '}'; }
<INITIAL,PP>","  { return ','; }

"bit"             { yylval.t.ft=FT_UNSIGNED; yylval.t.ft_sz=1; return TYPE; }
"float"           { yylval.t.ft=FT_FLOAT;    yylval.t.ft_sz=4; return TYPE; }
"double"          { yylval.t.ft=FT_FLOAT;    yylval.t.ft_sz=8; return TYPE; }
"int"[1-9]        { yylval.t.ft=FT_SIGNED;   yylval.t.ft_sz=strtol(&yytext[3], NULL, 10); return TYPE; }
"uint"[1-9]       { yylval.t.ft=FT_UNSIGNED; yylval.t.ft_sz=strtol(&yytext[4], NULL, 10); return TYPE; }
"int"[1-3][0-9]?  { yylval.t.ft=FT_SIGNED;   yylval.t.ft_sz=strtol(&yytext[3], NULL, 10); return TYPE; }
"uint"[1-3][0-9]? { yylval.t.ft=FT_UNSIGNED; yylval.t.ft_sz=strtol(&yytext[4], NULL, 10); return TYPE; }

<INITIAL,PP>\-[0-9]+        { yylval.v.i = strtol(yytext, NULL, 10); yylval.v.ft = FT_SIGNED; return INTEGER; }
<INITIAL,PP>\+?[0-9]+        { yylval.v.i = strtol(yytext, NULL, 10); yylval.v.ft = FT_UNSIGNED; return INTEGER; }
<INITIAL,PP>[\-\]?[0-9]+        { yylval.v.i = strtol(yytext, NULL, 10); yylval.v.ft = FT_SIGNED; return INTEGER; }
<INITIAL,PP>0b([01]+)           { yylval.v.i = strtol(&yytext[2], NULL, 2); yylval.v.ft = FT_UNSIGNED; return INTEGER; }
<INITIAL,PP>\+0b([01]+)         { yylval.v.i = strtol(&yytext[3], NULL, 2); yylval.v.ft = FT_UNSIGNED; return INTEGER; }
<INITIAL,PP>\-0b([01]+)         { yylval.v.i = -strtol(&yytext[3], NULL, 2); yylval.v.ft = FT_SIGNED; return INTEGER; }
<INITIAL,PP>\-0x[0-9A-Fa-f]+   { yylval.v.i = strtol(yytext, NULL, 16); yylval.v.ft = FT_SIGNED; return INTEGER; }
<INITIAL,PP>\+?0x[0-9A-Fa-f]+   { yylval.v.i = strtol(yytext, NULL, 16); yylval.v.ft = FT_UNSIGNED; return INTEGER; }
<INITIAL,PP>[-+]?([0-9]*\.?[0-9]+|[0-9]+\.)[f]?  { yylval.v.d = strtod(yytext, NULL); yylval.v.ft = FT_FLOAT; return FT_FLOAT; }

<INITIAL,PP>"\""[a-zA-Z_][a-zA-Z0-9_]*"\""  { yylval.s = strdup(&yytext[1]); yylval.s[yyleng-2] = '\0'; return STRING; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.s = strdup(yytext); return VAR; }

<INITIAL,PP>"//".*  { BEGIN(COMMENTS_LINE); }
<COMMENTS_LINE>\n   { ++line_num; BEGIN(INITIAL); }   // still have to increment line numbers inside of comments!
<INITIAL,PP>\/\*    { BEGIN(COMMENTS); } // start of a comment: go to a 'COMMENTS' state.
<COMMENTS>\*\/      { BEGIN(INITIAL); } // end of a comment: go back to normal parsing.
<COMMENTS>\n        { ++line_num; }   // still have to increment line numbers inside of comments!
<COMMENTS>.*        // nada

<INITIAL,PP>\n      { ++line_num; }
<INITIAL,PP>[ \t]   /* return nada */
<INITIAL,PP>.       { printf("Unkown: %c\n", *yytext); }
