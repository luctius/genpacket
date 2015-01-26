ACLOCAL_AMFLAGS=-I m4
#SUBDIRS = src test

AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS=-I$(top_srcdir)/utils -I. -I$(top_srcdir)/src
bin_PROGRAMS = genpacket

#SUBDIRS = skels

GIT_VERSION=$(shell git describe --abbrev=10 --dirty --always)

genpacket_program_sources = \
                    src/cmdline.c \
					$(gengen_sources) \
                    src/genpacket.tab.c \
                    src/genpacket.lex.c \
                    src/packet.c \
                    src/parser.c \
                    src/value.c \
                    src/gensrc.c \
					src/decoder.c \
                    src/debug_print.c \
					src/crc.c \
					src/json_printer.c

genpacket_SOURCES =  \
					$(genpacket_program_sources) \
                    src/main.c

genpacket_CFLAGS = $(lua_CFLAGS) -ggdb -O0 -Wall -Wextra -Wformat=2 -Wswitch-default -Wcast-align -DGIT_VERSION=\"$(GIT_VERSION)\" -Wpointer-arith -Wbad-function-cast -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings  -Wunreachable-code -Wstrict-aliasing=2 -Wwrite-strings -Wno-uninitialized #-Wno-aggressive-loop-optimizations -Wlogical-op
#-Wpadded  -Waggregate-return
#genpacket_LDADD = -lm libgp.la

src/cmdline.c src/cmdline.h: genpacket.ggo
	gengetopt --output-dir=src/ < $<


src/genpacket.tab.c: src/genpacket.y
	$(YACC) -d src/genpacket.y

src/genpacket.lex.c: src/genpacket.l
	$(LEX) src/genpacket.l

install-exec-hook:
	$(LN_S) $(DESTDIR)/bin/genpacket $(DESTDIR)/bin/packit




# Copyright (C) 1999-2008 Free Software Foundation, Inc.
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# if gengen is not installed we simply ignore the changes

SUFFIXES = .h_skel

if NO_GENGEN
#.h_skel.c:
#	echo "Not regenerating $@ since gengen is not installed"
# better not to use touch, otherwise we will create an empty file in
# the build directory
#	touch $@

else
GENERATE = $(GENGEN)

#src/skels/%.h: src/skels/%.h_skel
#src/skels/.c: .h_skel
$(gengen_sources): $(EXTRA_DIST)
	name="`echo $* | sed 's/^.*\///g'`"; \
	echo "$$name"; \
	$(GENERATE) -i $*.h_skel -F $*.h --output-format=c --separate-files --output-dir=$(srcdir)/src/skels/ --force -f $$name
	echo $(GENERATE) -i $< -F $*.h --output-format=c -f $$name  --separate-files --output-dir=$(srcdir) --force
endif



#noinst_LTLIBRARIES = src/skels/libgp.la

#libgp_la_SOURCES = $(BUILT_SOURCES)

gengen_sources = src/skels/header.h src/skels/header.c\
				src/skels/structs.c src/skels/structs.h \
				src/skels/send_function.c src/skels/send_function.h \
                src/skels/receive_callback.c src/skels/receive_callback.h \
                src/skels/source_skeleton.c src/skels/source_skeleton.h \
				src/skels/var_declaration.c src/skels/var_declaration.h

EXTRA_DIST = src/skels/header.h_skel \
			src/skels/structs.h_skel \
            src/skels/send_function.h_skel \
            src/skels/receive_callback.h_skel \
			src/skels/var_declaration.h_skel \
			src/skels/source_skeleton.h_skel
#$(BUILT_SOURCES) \
$(TESTS)

built-clean:
	cd @srcdir@ && rm -f $(BUILT_SOURCES)




TESTS = test/test_packit test/test_packit_output.test
check_PROGRAMS = test/test_packit
test_test_packit_SOURCES = test/test_packit.c $(genpacket_program_sources)
test_test_packit_CFLAGS = @check_CFLAGS@ -I src
test_test_packit_LDADD = @check_LIBS@

TEST_LOG_DRIVER = env AM_TAP_BATS='test/bats/bin/bats' $(SHELL) \
                  tap-driver.sh


AM_TESTS_ENVIRONMENT = \
					PATH=test/bats/bin:$$PATH; export PATH; \
					if [ ! -d test/bats/bin ]; then \
						git clone git@github.com:sstephenson/bats.git test/bats;\
					fi;
