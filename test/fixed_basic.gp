// test with
// echo -e "sddsdsblaaat\x10\xa\xb\xb\xc\xc\xc\xc\x32\x32\x32\x32\x16\x16\x8\x8\x8\x8\x8\x8\xc" | src/packit -i - -p test/fixed_basic.gp 
fixed packet "p2_fixed" size=21 {
    frame 0x10
    attribute
    attribute type=uint16
    attribute type=uint32
    data type=uint32 data_size=1
    data type=uint16 data_size=1
    data
    crc crc_8
}

fixed packet "p1_fixed" pipe=0 {
    frame 0x10
    attribute
    attribute
    attribute
    data type=float data_size=10
    crc crc_16 type=uint16
}

dynamic packet "p3_dynamic" {
    frame 0x10
    attribute
    size "sz"
    data data_size="sz"
    crc crc_8
}
