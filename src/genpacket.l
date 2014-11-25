%{

#include "genpacket.tab.h"
int line_num = 1;
%}

%option outfile="genpacket.lex.c"
%option noyywrap

%x COMMENTS
%x PACKET_PARAMS

%%

"fixed "            { return PK_TFIXED; }
"dynamic "          { return PK_TDYNAMIC; }
"calculated "       { return PK_TCALC; }
"packet "           { BEGIN(PACKET_PARAMS); return PACKET;}
<PACKET_PARAMS>"size"       { return PA_SIZE; }
<PACKET_PARAMS>"pipe"       { return PA_PIPE; }

"frame"             { return PO_FRAME; }
"attr"              { return PO_ATTR; }
"attribute"         { return PO_ATTR; }
"size"              { return PO_SIZE; }
"crc"               { return PO_CRC; }
"data"              { return PO_DATA; }

"type"              { return OP_TYPE; }
"data_width"        { return OP_DATAWIDTH; }
"datawidth"         { return OP_DATAWIDTH; }
"data_size"         { return OP_DATASIZE; }
"datasize"          { return OP_DATASIZE; }
"default"           { return OP_DEFAULT; }
"values"            { return OP_VALUES; }
"exclude"           { return OP_EXCLUDE; }
"start"             { return OP_START; }
"end"               { return OP_END; }

<INITIAL,PACKET_PARAMS>"="                 { return '='; }
<INITIAL,PACKET_PARAMS>"{"                 { BEGIN(INITIAL); return '{'; }
<INITIAL,PACKET_PARAMS>"}"                 { BEGIN(INITIAL); return '}'; }
","                 { return ','; }

"bit"               { yylval.string = strdup(yytext); return TYPE; }
"float"             { yylval.string = strdup(yytext); return TYPE; }
"double"            { yylval.string = strdup(yytext); return TYPE; }
"int"[1-9][0-9]?    { yylval.string = strdup(yytext); return TYPE; }
"uint"[1-9][0-9]?   { yylval.string = strdup(yytext); return TYPE; }

<INITIAL,PACKET_PARAMS>[\-\+0-9]+  { yylval.number = strtol(yytext, NULL, 10); return NUMBER; }
<INITIAL,PACKET_PARAMS>0b([01]+) { yylval.number = strtol(&yytext[2], NULL, 2); return NUMBER; }
<INITIAL,PACKET_PARAMS>\+0b([01]+) { yylval.number = strtol(&yytext[3], NULL, 2); return NUMBER; }
<INITIAL,PACKET_PARAMS>\-0b([01]+) { yylval.number = -strtol(&yytext[3], NULL, 2); return NUMBER; }
<INITIAL,PACKET_PARAMS>[\-\+]?0x[0-9A-Fa-f]+  { yylval.number = strtol(yytext, NULL, 16); return NUMBER; }

<INITIAL,PACKET_PARAMS>"\""[a-zA-Z_][a-zA-Z0-9_]*"\""  { yylval.string = strdup(yytext); return STRING; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.string = strdup(yytext); return VAR; }

<INITIAL,PACKET_PARAMS>"//".*  { return COMMENT; }
<INITIAL,PACKET_PARAMS>\/\*    { BEGIN(COMMENTS); } // start of a comment: go to a 'COMMENTS' state.
<COMMENTS>\*\/                 { BEGIN(INITIAL); } // end of a comment: go back to normal parsing.
<COMMENTS>\n                   { ++line_num; }   // still have to increment line numbers inside of comments!

<INITIAL,PACKET_PARAMS>\n      { ++line_num; }
<INITIAL,PACKET_PARAMS>[ \t]   /* return nada */
<INITIAL,PACKET_PARAMS>.       { printf("Unkown: %c\n", *yytext); }
