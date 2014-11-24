AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS=-I$(top_srcdir)/utils -I.
bin_PROGRAMS = genpacket

GIT_VERSION:=$(shell git describe --abbrev=10 --dirty --always)

genpacket_SOURCES =  \
                    cmdline.c \
                    genpacket.tab.c \ 
                    genpacket.lex.c \
                    main.c

genpacket_CFLAGS = $(lua_CFLAGS) -ggdb -O0 -Wall -Wextra -Wformat=2 -Wswitch-default -Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings  -Wunreachable-code -Wstrict-aliasing=2 -Wwrite-strings -Wno-aggressive-loop-optimizations -Wlogical-op -Wno-uninitialized -DGIT_VERSION=\"$(GIT_VERSION)\"
#-Wpadded  -Waggregate-return
genpacket_LDADD = -lm

cmdline.c cmdline.h: ../genpacket.ggo
	sed -i "s:usr_prefix:${prefix}:g" $<
	gengetopt < $<


genpacket.tab.c: genpacket.y
    bison -d genpacket.y

lex.yy.c: genpacket.l
    flex genpacket.l
