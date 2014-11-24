%{

#include <stdio.h>

int yylex(void);
int yyerror(const char *s);

%}

%token PACKET_TYPE_FIXED PACKET_TYPE_DYNAMIC PACKET_TYPE_CALCULATED
%token PACKET PACKET_START PACKET_END
%token PACKET_ATTR_FRAME PACKET_ATTR_ATTRIBUTE PACKET_ATTR_OPTION_TYPE


%token VARIABLE
%token ASSIGN
%token TYPE
%token NUMBER
%token ADD SUB MUL DIV ABS
%token CP OP
%token EOL COMMENT

%%

packet:  /* nothing */
    | packet_type {printf("done\n");}
    | COMMENT EOL       {}
;

packet_type:
    | PACKET_TYPE_FIXED PACKET VARIABLE EOL     { printf("fixed packet\n"); }
    | PACKET_TYPE_DYNAMIC PACKET VARIABLE EOL     { printf("dynamic packet\n"); }
    | PACKET_TYPE_CALCULATED PACKET VARIABLE EOL     { printf("calculated packet\n"); }

option:
    | PACKET_ATTR_FRAME variable PACKET_ATTR_OPTION_TYPE ASSIGN type

type: TYPE
    |               { printf("type\n"); }

variable: VARIABLE
    |               { printf("variable\n"); }

%%

int yyerror(const char *s) {
    fprintf(stderr, "error: %s\n",s);
    return 0;
}
