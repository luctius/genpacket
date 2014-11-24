%{

#include <stdio.h>

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

%%

int yyerror(char *s) {
    fprintf(stderr, "error: %s\n",s);
    return 0;
}
