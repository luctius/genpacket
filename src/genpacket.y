%{

#include <stdio.h>

int yylex(void);
void yyerror(const char *s);

#define YYERROR_VERBOSE

%}

%union
{
    int number;
    char *string;
}

%token PACKET_TYPE_FIXED PACKET_TYPE_DYNAMIC PACKET_TYPE_CALCULATED
%token PACKET PACKET_START PACKET_END
%token PACKET_ATTR_FRAME PACKET_ATTR_ATTRIBUTE PACKET_ATTR_OPTION_TYPE


%token <string> VARIABLE
%token ASSIGN
%token <number> TYPE
%token <number> NUMBER
%token EOL COMMENT

%%

input: option | input option;

option:
    | PACKET_ATTR_FRAME VARIABLE            { printf("frame: %s\n", $2); }
    | PACKET_ATTR_ATTRIBUTE VARIABLE        { printf("attr: %s\n", $2); }
    | COMMENT EOL                           {}
    | EOL                                   {}
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n",s);
}
