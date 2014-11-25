%{

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

"bit"             { yylval.s = strdup(yytext); return TYPE; }
"float"           { yylval.s = strdup(yytext); return TYPE; }
"double"          { yylval.s = strdup(yytext); return TYPE; }
"int"[1-9]        { yylval.s = strdup(yytext); return TYPE; }
"uint"[1-9]       { yylval.s = strdup(yytext); return TYPE; }
"int"[1-3][0-9]?  { yylval.s = strdup(yytext); return TYPE; }
"uint"[1-3][0-9]? { yylval.s = strdup(yytext); return TYPE; }

<INITIAL,PP>[\-\]?[0-9]+  { yylval.i = strtol(yytext, NULL, 10); return INTEGER; }
<INITIAL,PP>0b([01]+)   { yylval.i = strtol(&yytext[2], NULL, 2); return INTEGER; }
<INITIAL,PP>\+0b([01]+) { yylval.i = strtol(&yytext[3], NULL, 2); return INTEGER; }
<INITIAL,PP>\-0b([01]+) { yylval.i = -strtol(&yytext[3], NULL, 2); return INTEGER; }
<INITIAL,PP>[\-\+]?0x[0-9A-Fa-f]+  { yylval.i = strtol(yytext, NULL, 16); return INTEGER; }
<INITIAL,PP>[-+]?([0-9]*\.?[0-9]+|[0-9]+\.)[f]?  { yylval.f = strtod(yytext, NULL); return FLOAT; }

<INITIAL,PP>"\""[a-zA-Z_][a-zA-Z0-9_]*"\""  { yylval.s = strdup(yytext); return STRING; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.s = strdup(yytext); return VAR; }

<INITIAL,PP>"//".*  { BEGIN(COMMENTS_LINE); }
<COMMENTS_LINE>\n   { ++line_num; BEGIN(INITIAL); }   // still have to increment line numbers inside of comments!
<INITIAL,PP>\/\*    { BEGIN(COMMENTS); } // start of a comment: go to a 'COMMENTS' state.
<COMMENTS>\n        { ++line_num; }   // still have to increment line numbers inside of comments!
<COMMENTS>.*        // nada
<COMMENTS>\*\/      { BEGIN(INITIAL); } // end of a comment: go back to normal parsing.

<INITIAL,PP>\n      { ++line_num; }
<INITIAL,PP>[ \t]   /* return nada */
<INITIAL,PP>.       { printf("Unkown: %c\n", *yytext); }
