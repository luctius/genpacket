// test with
// echo -e "sddsdsblaaat\x10\xa\xb\xb\xc\xc\xc\xc\x32\x32\x32\x32\x16\x16\x0\x0\x80\x3fheyjow\x30a\x1\x2\x3\x61\x20" | src/packit -i - -p test/fixed_basic.gp
// 100a0b0b0c0c0c0c3232323216160000803f6865796a6f773061010203
fixed packet "p0_fixed" size=31 {
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
// Test with
// echo -e "\xa\xb\xc\x0\x0\x80\x3f\xa\x82\xdf\x19\xdf" | src/packit -i - -p test/fixed_basic.gp -n1
fixed packet "p1_fixed" size=12 {
    attribute
    attribute
    attribute
    data type=float data_size=1
    attribute
    crc crc_32 type=uint32
}

fixed packet "p2_fixed" pipe=0 {
    frame 0x10
    attribute
    attribute
    attribute
    data type=float data_size=10
    crc crc_16 type=uint16
}

// test with
// echo -e "sddsdsblaaat\x10\xa\x3\x4\xa\x1\x2\x3\x4\xa\x1\x2\x3\xb7" | src/packit -i - -p test/fixed_basic.gp -n3
calculated packet "p3_calculated" {
    frame 0x10
    attribute
    size "sz2"
    size "sz"
    attribute
    data data_size="sz"
    attribute
    data data_size="sz2"
    crc crc_8
}
