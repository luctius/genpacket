packet examples:



struct packet{
    enum type = {fixed,calculated,dynamic},
    char *name,
    int size;
    int pipe_nr;
    struct field[10] {
        enum field_type = {frame,attr,size,data,crc},
        enum type type = {uint8, bit,...},
        struct param = {
            enum param_type,
            char *cparam,
            struct field *fparam,
        },
        enum method = {...}
        enum type data_width,
        int values[],
        int exclude_count,
        struct field *exclude,
        struct field *start,
        struct field *end,
        struct type {
            ...
        } default,
        struct type frame_val,
    },
};


[fixed/calculated/dynamic] packet [name] size=<packet_size in bytes> pipe=<pipe_nr>
{
    frame [val] type=<type> name=<name>
    attr(ibute) [name] type=<type> default=<value> values=<value1>,<value2>
    size type=<type> data_width=<type> exclude=<attr1>,<attrN> name=<name>
    data <param> name=<name> exclude=<attr1>,<attrN> type=<type> data_width=<type> escape=<esc> escape_values=<e1>,<eN> escape_op=<func> escape_param=<param>
    crc [method] start=<start_attribute> end=<end_attribute> exclude=<attr1>,<attrN> type=<type> name=<name>
}

fixed packet simple_packet size=0x10
{
    frame name=id 0xDE /* type uint8 */
    data /* type uint8 */
}

dynamic packet serial_p
{
    frame 0x7E name=start

    attr protocol
    attr sequence_nr
    attr packet_format
    data escape=0x7D escape_values=0x7D,0x7E escape_op=xor escape_param=0x20
    crc crc16

    frame 0x7E name=stop
}

dynamic packet serial
{
    frame xon name=start
    data
    frame xoff name=stop
}

calculated packet modbus_rtu_request
{
    attribute slave_address
    attribute function_code values=0x1,0x2,0x3
    attribute starting_address type=uint16
    attribute nr_address type=uint16
    crc crc16 start=slave_address end=nr_address type=uint16
}

calculated packet modbus_rtu_response
{
    attribute slave_address
    attribute function_code values=0x1,0x2,0x3
    size name=byte_count
    data size type=uint16
    crc crc16 start=slave_address end=data type=uint16
}

calculated packet udp
{
    attribute source type=uint16
    attribute destination type=uint16
    size name=length type=uint16
    crc crc16 type=uint16 name=checksum
    data size exclude=source,destination,length,checksum
}

types:
bit
char
uint2 -> uint32
int2 -> int32
float
double

crc methods
crc8
crc16
callback


maybe?:
    data_attr    [size] exclude=<attr1>,<attrN> data_width=<type> type=<type> name=<name>
    data_fixed   [len] type=<type> name=<name>
    data_dynamic [value] trigger=<t1>,<t2> op=<func> param=<param> type=<type> name=<name>