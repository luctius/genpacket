fixed packet "p2_fixed" size=20 {
    frame 0x10
    attribute
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
