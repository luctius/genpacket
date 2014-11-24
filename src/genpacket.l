%{

#include "genpacket.tab.h"

%}

%option outfile="genpacket.lex.c"
%option noyywrap

%%

"fixed"             { return PACKET_TYPE_FIXED; }
"dynamic"           { return PACKET_TYPE_DYNAMIC; }
"calculated"        { return PACKET_TYPE_CALCULATED; }
"packet"            { return PACKET;}
"{"                 { return PACKET_START; }
"}"                 { return PACKET_END; }

"frame"             { return PACKET_ATTR_FRAME; }
"attr"              { return PACKET_ATTR_ATTRIBUTE; }
"attribute"         { return PACKET_ATTR_ATTRIBUTE; }

"type"              { return PACKET_ATTR_OPTION_TYPE; }
"="                 { return ASSIGN; }

"bit"               { yylval = 1; return TYPE; }
"int[0-9].[0-9]?"   { return TYPE; }
"uint[0-9].[0-9]?"  { return TYPE; }

[a-zA-Z_][a-zA-Z0-9_]*  { return VARIABLE; }


"#".*  { return COMMENT; }
[0-9]+  { yylval = atoi(yytext); return NUMBER; }
\n      { printf("\n"); return EOL; }
[ \t]   /* return nada */
.       { printf("Unkown: %c\n", *yytext); }
