%{

#include "genpacket.tab.h"
int line_num = 1;
%}

%option outfile="genpacket.lex.c"
%option noyywrap

%x COMMENTS

%%

"fixed "            { return PK_TFIXED; }
"dynamic "          { return PK_TDYNAMIC; }
"calculated "       { return PK_TCALC; }
"packet "           { return PACKET;}
"size"[ ]*"="       { return PA_SIZE; }
"pipe"[ ]*"="       { return PA_PIPE; }

"frame"             { return PO_FRAME; }
"attr"              { return PO_ATTR; }
"attribute"         { return PO_ATTR; }
"size"              { return PO_SIZE; }
"crc"               { return PO_CRC; }
"data"              { return PO_DATA; }

"type"              { return OP_TYPE; }
"data_width"        { return OP_DATAWIDTH; }
"datawidth"         { return OP_DATAWIDTH; }
"default"           { return OP_DEFAULT; }
"values"            { return OP_VALUES; }
"exclude"           { return OP_EXCLUDE; }
"start"             { return OP_START; }
"end"               { return OP_END; }

"="                 { return '='; }
"{"                 { return '{'; }
"}"                 { return '}'; }
","                 { return ','; }

"bit"               { yylval.string = strdup(yytext); return TYPE; }
"float"             { yylval.string = strdup(yytext); return TYPE; }
"double"            { yylval.string = strdup(yytext); return TYPE; }
"int"[1-9][0-9]?    { yylval.string = strdup(yytext); return TYPE; }
"uint"[1-9][0-9]?   { yylval.string = strdup(yytext); return TYPE; }

0x[0-9A-Fa-f]+  { yylval.number = strtol(yytext, NULL, 16); return NUMBER; }

"\""[a-zA-Z_][a-zA-Z0-9_]*"\""  { yylval.string = strdup(yytext); return STRING; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.string = strdup(yytext); return VAR; }

\n                      { ++line_num; }

"#".*  { return COMMENT; }
\/\*                    { // start of a comment: go to a 'COMMENTS' state.
	                        BEGIN(COMMENTS);
	                    }
<COMMENTS>\*\/          {   // end of a comment: go back to normal parsing.
	                        BEGIN(INITIAL);
	                    }
<COMMENTS>\n            { ++line_num; }   // still have to increment line numbers inside of comments!
<COMMENTS>.             ;   // ignore every other character while we're in this state

[0-9]+  { yylval.number = strtol(yytext, NULL, 10); return NUMBER; }
[ \t\n]   /* return nada */
.       { printf("Unkown: %c\n", *yytext); }
