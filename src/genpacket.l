%{

#include <stdlib.h>
#include <limits.h>

#include "packet.h"
#include "genpacket.tab.h"

int line_num = 1;
int comment_caller = 0;
%}

%option outfile="src/genpacket.lex.c"
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
"float"           { yylval.t.ft=FT_FLOAT;    yylval.t.ft_sz=(sizeof(float) * CHAR_BIT); return TYPE; }
"double"          { yylval.t.ft=FT_FLOAT;    yylval.t.ft_sz=(sizeof(double) * CHAR_BIT); return TYPE; }
"int"[1-9][0-9]?  { yylval.t.ft=FT_SIGNED;   yylval.t.ft_sz=strtol(&yytext[3], NULL, 10); return TYPE; }
"uint"[1-9][0-9]? { yylval.t.ft=FT_UNSIGNED; yylval.t.ft_sz=strtol(&yytext[4], NULL, 10); return TYPE; }
"char"            { yylval.t.ft=FT_CHAR;     yylval.t.ft_sz=(sizeof(char) * CHAR_BIT); return TYPE; }

<INITIAL,PP>\-[0-9]+            { yylval.v = v_set_i(strtol(yytext, NULL, 10) ); return INTEGER; }
<INITIAL,PP>\+?[0-9]+           { yylval.v = v_set_u(strtol(yytext, NULL, 10) ); return INTEGER; }
<INITIAL,PP>[\-\]?[0-9]+        { yylval.v = v_set_i(strtol(yytext, NULL, 10) ); return INTEGER; }
<INITIAL,PP>0b([01]+)           { yylval.v = v_set_u(strtol(&yytext[2], NULL, 2) ); return INTEGER; }
<INITIAL,PP>\+0b([01]+)         { yylval.v = v_set_u(strtol(&yytext[3], NULL, 2) ); return INTEGER; }
<INITIAL,PP>\-0b([01]+)         { yylval.v = v_set_i(-strtol(&yytext[3], NULL, 2) ); return INTEGER; }
<INITIAL,PP>\-0x[0-9A-Fa-f]+    { yylval.v = v_set_i(strtol(yytext, NULL, 16) ); return INTEGER; }
<INITIAL,PP>\+?0x[0-9A-Fa-f]+   { yylval.v = v_set_u(strtol(yytext, NULL, 16) ); return INTEGER; }
<INITIAL,PP>[-+]?([0-9]*\.?[0-9]+|[0-9]+\.)[f]?  { yylval.v = v_set_f(strtod(yytext, NULL) ); return FT_FLOAT; }

<INITIAL,PP>"\""[a-zA-Z_][a-zA-Z0-9_]*"\""  { yylval.s = strdup(&yytext[1]); yylval.s[yyleng-2] = '\0'; return STRING; }

"crc_"[a-zA-Z0-9_]*  { yylval.s = strdup(yytext); return CRC_VAR; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.s = strdup(yytext); return VAR; }

<INITIAL,PP>"//".*  { BEGIN(COMMENTS_LINE);  return ENDL; }
<COMMENTS_LINE>\n   { ++line_num; BEGIN(INITIAL); }                // still have to increment line numbers inside of comments!
<INITIAL>"/*"       { comment_caller = INITIAL; BEGIN(COMMENTS); } // start of a comment: go to a 'COMMENTS' state.
<PP>"/*"            { comment_caller = PP;      BEGIN(COMMENTS); } // start of a comment: go to a 'COMMENTS' state.
<COMMENTS>"*/"      { BEGIN(comment_caller); }                     // end of a comment: go back to normal parsing.
<COMMENTS>\n        { ++line_num; }                                // still have to increment line numbers inside of comments!
<COMMENTS>[^*\n]*
<COMMENTS>"*"+[^*/\n]*

<INITIAL,PP>\n      { ++line_num; return ENDL; }
<INITIAL,PP>[ \t]   /* return nada */
<INITIAL,PP>.       { printf("Unkown: %c\n", *yytext); }
