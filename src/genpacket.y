%{

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

int yylex(void);
void yyerror(const char *s, ...);

#define YYERROR_VERBOSE
extern int line_num;
bool has_parse_error = false;
%}

%output "genpacket.tab.c"

%code requires {
#include "packet.h"
#include "parser.h"
}


%union
{
    char *s;
    struct value v;
    struct type t;
}

%token PK_TFIXED PK_TDYNAMIC PK_TCALC
%token PACKET PA_SIZE PA_PIPE
%token PO_FRAME PO_ATTR PO_SIZE PO_CRC PO_DATA
%token OP_TYPE OP_DATAWIDTH OP_DEFAULT OP_VALUES OP_EXCLUDE OP_START OP_END OP_DATASIZE

%token <s> VAR CRC_VAR STRING
%token <t> TYPE
%token <v> INTEGER FLOAT

%%

input: packet 
    | input packet
;

packet: 
    | packet_header packet_option_list '{' option_list '}'
    | packet_header '{' option_list '}'
;

packet_header: packet_option_list option_list
    | PK_TFIXED PACKET STRING    { cb_new_packet(PT_FIXED, $3); }
    | PK_TDYNAMIC PACKET STRING  { cb_new_packet(PT_DYNAMIC, $3); }
    | PK_TCALC PACKET STRING     { cb_new_packet(PT_CALCULATED, $3); }
;

packet_option_list: packet_option
    | packet_option_list packet_option
;

packet_option:
    | PA_SIZE '=' INTEGER       { cb_pa_size($3); }
    | PA_PIPE '=' INTEGER       { cb_pa_pipe($3); }
;

option_list: option
    | option_list option
;

option: 
    | PO_FRAME STRING INTEGER   { cb_frame_option($3); cb_op_name($2); } frame_plist 
    | PO_FRAME INTEGER          { cb_frame_option($2); } frame_plist 

    | PO_ATTR STRING            { cb_attr_option(); cb_op_name($2); } attr_plist  
    | PO_ATTR                   { cb_attr_option(); } attr_plist  

    | PO_SIZE STRING            { cb_size_option(); cb_op_name($2); } size_plist  
    | PO_SIZE                   { cb_size_option(); } size_plist

    | PO_CRC STRING CRC_VAR     { cb_crc_option($3); cb_op_name($2); } crc_plist  
    | PO_CRC CRC_VAR            { cb_crc_option($2); } crc_plist

    | PO_DATA STRING            { cb_data_option(); cb_op_name($2); } data_plist  
    | PO_DATA                   { cb_data_option(); } data_plist
;

frame_plist: f_param
    | frame_plist f_param
;

f_param:
    | op_type
;

attr_plist: a_param
    | attr_plist a_param
;

a_param:
    | op_type
    | op_default
    | op_values
;

size_plist: s_param
    | size_plist s_param
;

s_param:
    | op_type
    | op_datawidth
    | op_exclude
;

crc_plist: c_param
    | crc_plist c_param
;

c_param:
    | op_type
    | op_start
    | op_end
    | op_exclude
;

data_plist: d_param
    | data_plist d_param
;

d_param:
    | op_type
    | op_datawidth
    | op_datasize
    | op_exclude
;

op_type: OP_TYPE
    | OP_TYPE '=' TYPE          { cb_op_type($3); }
;

op_datawidth: OP_DATAWIDTH
    | OP_DATAWIDTH '=' TYPE     { cb_op_datawidth_type($3); }
    | OP_DATAWIDTH '=' INTEGER  { cb_op_datawidth_v($3); }
;

op_default: OP_DEFAULT
    | OP_DEFAULT '=' INTEGER    { cb_op_default_v($3); }
    | OP_DEFAULT '=' FLOAT      { cb_op_default_v($3); }
;

op_values: OP_VALUES
    | OP_VALUES '=' INTEGER     { cb_op_values_v($3); }
    | op_values ',' INTEGER     { cb_op_values_v($3); }
    | OP_VALUES '=' FLOAT       { cb_op_values_v($3); }
    | op_values ',' FLOAT       { cb_op_values_v($3); }
;

op_start: OP_START
    | OP_START '=' STRING       { cb_op_start($3); }
    | op_start ',' STRING       { cb_op_start($3); }
;

op_end: OP_END
    | OP_END '=' STRING         { cb_op_end($3); }
    | op_end ',' STRING         { cb_op_end($3); }
;

op_exclude: OP_EXCLUDE
    | OP_EXCLUDE '=' STRING     { cb_op_exclude($3); }
    | op_exclude ',' STRING     { cb_op_exclude($3); }
;

op_datasize: OP_DATASIZE
    | OP_DATASIZE '=' INTEGER   { cb_op_datasize_v($3); }
    | OP_DATASIZE '=' STRING    { cb_op_datasize_string($3); }
;

%%

#include <stdarg.h>

void yyerror(const char *s, ...) {
    va_list args;
    va_start(args, s);

    fprintf(stderr, "[line: %d] ", line_num);
    vfprintf(stderr, s, args);
    fprintf(stderr, ".\n");

    has_parse_error = true;
}

