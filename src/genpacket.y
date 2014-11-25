%{

#include <stdio.h>

int yylex(void);
void yyerror(const char *s);

#define YYERROR_VERBOSE
extern int line_num;

%}

%union
{
    int number;
    char *string;
}

%token PK_TFIXED PK_TDYNAMIC PK_TCALC
%token PACKET PA_SIZE PA_PIPE
%token PO_FRAME PO_ATTR PO_SIZE PO_CRC PO_DATA
%token OP_TYPE OP_DATAWIDTH OP_DEFAULT OP_VALUES OP_EXCLUDE OP_START OP_END OP_DATASIZE


%token <string> VAR STRING TYPE
%token <number> NUMBER
%token EOL COMMENT

%%

input: packet 
    | input packet
;

packet: 
    | packet_header packet_option_list '{' option_list '}'
    | packet_header '{' option_list '}'
    | COMMENT
;

packet_header: packet_option_list option_list
    | PK_TFIXED PACKET STRING    { printf("fixed packet: %s\n", $3); }
    | PK_TDYNAMIC PACKET STRING  { printf("dynamic packet: %s\n", $3); }
    | PK_TCALC PACKET STRING     { printf("calculated packet: %s\n", $3); }
;

packet_option_list: packet_option
    | packet_option_list packet_option
;

packet_option:
    | PA_SIZE '=' NUMBER    { printf("size: %d\n", $3); }
    | PA_PIPE '=' NUMBER    { printf("pipe: %d\n", $3); }
;

option_list: option
    | option_list option
;

option: 
    | PO_FRAME STRING NUMBER frame_plist { printf("frame %s: %d\n", $2, $3); }
    | PO_FRAME NUMBER frame_plist { printf("frame default: %d\n", $2); }
    | PO_ATTR STRING attr_plist  { printf("attr: %s\n", $2); }
    | PO_ATTR attr_plist  { printf("attr default\n"); }
    | PO_SIZE STRING size_plist  { printf("size: %s\n", $2); }
    | PO_SIZE size_plist  { printf("size default\n"); }
    | PO_CRC STRING VAR crc_plist  { printf("crc: %s: %s\n", $2, $3); }
    | PO_CRC VAR crc_plist  { printf("crc default: %s\n", $2); }
    | PO_DATA STRING data_plist  { printf("data: %s\n", $2); }
    | PO_DATA data_plist  { printf("data default\n"); }
    | PO_DATA STRING VAR data_plist  { printf("data: %s\n", $2); }
    | PO_DATA VAR data_plist  { printf("data default\n"); }
    | COMMENT
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
    | OP_TYPE '=' TYPE  { printf("type: %s\n", $3); }
;

op_datawidth: OP_DATAWIDTH
    | OP_DATAWIDTH '=' TYPE  { printf("data_width: %s\n", $3); }
;

op_default: OP_DEFAULT
    | OP_DEFAULT '=' NUMBER  { printf("default: %d\n", $3); }
;

op_values: OP_VALUES
    | OP_VALUES '=' NUMBER   {printf("values: %d\n", $3); }
    | op_values ',' NUMBER   {printf("values: %d\n", $3); }
;

op_start: OP_START
    | OP_START '=' STRING  {printf("start: %s\n", $3); }
    | op_start ',' STRING  {printf("start: %s\n", $3); }
;

op_end: OP_END
    | OP_END '=' STRING  {printf("end: %s\n", $3); }
    | op_end ',' STRING  {printf("end: %s\n", $3); }
;

op_exclude: OP_EXCLUDE
    | OP_EXCLUDE '=' STRING  {printf("exclude: %s\n", $3); }
    | op_exclude ',' STRING  {printf("exclude: %s\n", $3); }
;

op_datasize: OP_DATASIZE
    | OP_DATASIZE '=' NUMBER  { printf("data_size: %d\n", $3); }
    | OP_DATASIZE '=' STRING  { printf("data_size: %s\n", $3); }
;

%%

void yyerror(const char *s) {
    
    fprintf(stderr, "error: %s on line %d\n",s,line_num);
}
