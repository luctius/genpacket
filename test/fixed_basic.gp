// test with
// echo -e "sddsdsblaaat\x10\xa\xb\xb\xc\xc\xc\xc\x32\x32\x32\x32\x16\x16\x0\x0\x80\x3fheyjow\x30a\x1\x2\x3\x61\x20" | src/packit -i - -p test/fixed_basic.gp
// 100a0b0b0c0c0c0c3232323216160000803f6865796a6f773061010203
fixed packet "p2_fixed" size=31 {
    frame 0x10
    attribute
    attribute type=uint16
    attribute type=uint32
    data type=uint32 data_size=1
    data type=uint16 data_size=1
    data type=float data_size=1
    data type=char data_size=3
    data type=char data_size=3
    frame 0x30
    data type=char data_size=1
    attribute
    data
    crc crc_8
    frame 0x20
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
