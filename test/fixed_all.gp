/*[fixed/calculated/dynamic] packet ["name"] size=<packet_size in bytes> pipe=<pipe_nr>
{
    frame <"name"> [val] type=<type>
    attr(ibute) <"name"> type=<type> default=<value> values=<value1>,<value2>
    size <"name"> type=<type> data_width=<type> exclude=<"attr1">,<"attrN">
    data <"name"> <param> exclude=<"attr1">,<"attrN"> type=<type> data_width=<type> escape=<esc> escape_values=<e1>,<eN> escape_op=<func> escape_param=<param>
    crc <"name"> [method] start=<"start_attribute"> end=<"end_attribute"> exclude=<"attr1">,<"attrN"> type=<type> poly=<poly> xor_in=<xor_in> xor_out=<xor_out> reflect_in=<reflect_in> reflect_out=<reflect_out>
}
*/

        .width          = 32,
        .poly           = 0x04c11db7,
        .xor_in         = 0xffffffff,
        .reflect_in     = true,
        .xor_out        = 0xffffffff,
        .reflect_out    = true,

fixed packet "p1" size=10 pipe=0 /*a*/ {
    frame /*b*/ 0x10 //comment

    /*a*a*a*/
    frame 0x10
    frame "frame2" 0b01010
    frame "frame3" 1 type=uint16
	frame "frame4" 2 type=uint16

    attribute
    attribute "test1"
    attribute "test2" type=uint8
    attribute "test3" type=uint8 default=0b11
    attribute "test4" type=int8 default=0x11 values=-0x1,0x2,-3

    size
    size "size1"
    size "size2" type=uint32
    size "size3" type=uint32 data_width=uint16
    size "size4" type=uint32 data_width=uint16 exclude="test1"
    size "size5" type=uint32 data_width=uint16 exclude="test2","size1"

    crc crc_16
    crc "crc1" crc_8
    crc "crc2" crc_callback type=bit
    crc "crc3" crc_callback type=bit start="test1"
    crc "crc4" crc_callback type=bit start="test2" end="size1"
    crc "crc5" crc_callback type=bit start="test3" end="size1" exclude="frame3"
    crc "crc6" crc_callback type=bit start="test4" end="size1" exclude="data9","crc1"
    crc custom_crc type=uint16 start="test4" end="size1" exclude="data9","crc1",poly=0x07 xor_in=0 xor_out=0 reflect_in=false reflect_out=false

    data
    data "data1"
    data "data2" data_size="size2"
    data "data3" exclude="size1","size5"
    data "data4" exclude="size1","size5" type=uint10 
    data "data5" exclude="size1","size5" type=uint10 data_width=uint8
    data "data6" exclude="size1","size5" type=uint10 data_width=uint8 data_size=0x11
    data "data7" exclude="size1","size5" type=uint10 data_width=uint8 data_size=11
    data "data8" exclude="size1","size5" type=uint10 data_width=uint8 data_size="crc1"
    data "data9" exclude="size1","size5" type=uint10 data_width=uint8 //escape=<esc> escape_values=<e1>,<eN> escape_op=<func> escape_param=<param>
    
}


