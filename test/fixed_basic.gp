/*[fixed/calculated/dynamic] packet ["name"] size=<packet_size in bytes> pipe=<pipe_nr>
{
    frame <"name"> [val] type=<type>
    attr(ibute) <"name"> type=<type> default=<value> values=<value1>,<value2>
    size <"name"> type=<type> data_width=<type> exclude=<"attr1">,<"attrN">
    data <"name"> <param> exclude=<"attr1">,<"attrN"> type=<type> data_width=<type> escape=<esc> escape_values=<e1>,<eN> escape_op=<func> escape_param=<param>
    crc <"name"> [method] start=<"start_attribute"> end=<"end_attribute"> exclude=<"attr1">,<"attrN"> type=<type>
}
*/

fixed packet "p1" size=10 pipe=0 {
    frame 0x10 //comment

    /*a*a*a*/
    frame 0x10
    frame "frame2" -0b01010
    frame "frame3" 1 type=uint16
	frame "frame4" 2 type=uint16

    attribute
    attribute "test1"
    attribute "test2" type=uint8
    attribute "test3" type=uint8 default=0b11
    attribute "test4" type=uint8 default=0x11 values=-0x1,0x2,-3

    size
    size "size1"
    size "size2" type=uint32
    size "size3" type=uint32 data_width=uint16
    size "size4" type=uint32 data_width=uint16 exclude="test"
    size "size5" type=uint32 data_width=uint16 exclude="test","size1"

    crc crc_16
    crc "crc1" crc_8
    crc "crc2" crc_callback type=bit
    crc "crc3" crc_callback type=bit start="test"
    crc "crc4" crc_callback type=bit start="test" end="size1"
    crc "crc5" crc_callback type=bit start="test" end="size1" exclude="start"
    crc "crc6" crc_callback type=bit start="test" end="size1" exclude="start","crc1"

    data
    data "data1"
    data "data2" 
    data "data3" exclude="size1","size5"
    data "data4" exclude="size1","size5" type=uint10
    data "data5" exclude="size1","size5" type=uint10 data_width=uint8
    data "data6" exclude="size1","size5" type=uint10 data_width=uint8 data_size=0x11
    data "data7" exclude="size1","size5" type=uint10 data_width=uint8 data_size=11
    data "data8" exclude="size1","size5" type=uint10 data_width=uint8 data_size="crc1"
    data "data9" exclude="size1","size5" type=uint10 data_width=uint8 //escape=<esc> escape_values=<e1>,<eN> escape_op=<func> escape_param=<param>
}

