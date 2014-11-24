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

%token PK_TFIXED PK_TDYNAMIC PK_TCALC
%token PACK PSTART PEND
%token PA_FRAME PA_ATTR 
%token OP_TYPE


%token <string> VAR
%token ASSIGN
%token <number> TYPE
%token <number> NUMBER
%token EOL COMMENT

%%

input: packet | input packet;

packet: 
    | packet_header PSTART option_list PEND     {}

packet_header:
    | PK_TFIXED PACK VAR    { printf("fixed packet: %s\n", $3); }
    | PK_TDYNAMIC PACK VAR  { printf("dynamic packet: %s\n", $3); }
    | PK_TCALC PACK VAR     { printf("calculated packet: %s\n", $3); }
    | COMMENT {}
;

option_list: option
    | option_list option
;

option:
    | PA_FRAME VAR { printf("frame: %s\n", $2); }
    | PA_ATTR VAR { printf("attr: %s\n", $2); }
    | COMMENT {}
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n",s);
}
