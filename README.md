### GenPacket
*GenPacket* is a generator for binary packet protocols much like gengetopt is for getopt.
With a simple dsl it aims to make implementing a binary custom packet format a lot easyier, without the need to write the skeleton code over and over again.

The range of supported package types is from simple fixed size packets to packets with a start stop frame and escaping of data.

Examples can be found