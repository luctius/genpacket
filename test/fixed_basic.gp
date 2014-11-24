fixed packet "p1" size=10 pipe=0 {
    frame 0x10
    frame "start1" 0x10
    frame "start2" 0x11 type=uint16

    attribute
    attribute "test1"
    attribute "test2" type=uint8
    attribute "test3" type=uint8 default=11
    attribute "test4" type=uint8 default=0x11 values=1,0x2,3

    size
    size "size1"
    size "size2" type=uint32
    size "size3" type=uint32 data_width=uint16
    size "size4" type=uint32 data_width=uint16 exclude="test"
    size "size5" type=uint32 data_width=uint16 exclude="test","size1"

    crc crc16
    crc "crc1" crc8
    crc "crc2" callback type=bit
    crc "crc3" callback type=bit start="test"
    crc "crc4" callback type=bit start="test" end="size1"
    crc "crc5" callback type=bit start="test" end="size1" exclude="start"
    crc "crc6" callback type=bit start="test" end="size1" exclude="start","crc1"
}

dynamic packet "p2" { frame 0x10 attribute size crc crc16 }

