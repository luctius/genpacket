ACLOCAL_AMFLAGS=-I m4
#SUBDIRS = src test

AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS=-I$(top_srcdir)/utils -I. -I$(top_srcdir)/src
AM_YFLAGS=-d
bin_PROGRAMS = src/genpacket

#SUBDIRS = skels
SUFFIXES = .c .h .y .l .h_skel

GIT_VERSION := "$(shell git describe --abbrev=10 --dirty --always)"

genpacket_program_sources = \
                    $(gengen_sources) \
                    src/cmdline.c \
                    src/cmdline_packit.c \
                    src/genpacket.tab.c \
                    src/genpacket.lex.c \
                    src/packet.c \
                    src/parser.c \
                    src/value.c \
                    src/generate.c \
                    src/genhdr.c \
                    src/gensrc.c \
                    src/gentools.c \
                    src/decoder.c \
                    src/debug_print.c \
                    src/crc_custom.c \
                    src/crc.c \
                    src/json_printer.c

src_genpacket_SOURCES =  \
					$(genpacket_program_sources) \
                    src/main.c

src_genpacket_CFLAGS = -ggdb -O0 -Wall -Wextra -Wformat=2 -Wswitch-default -Wcast-align -DGIT_VERSION=\"$(GIT_VERSION)\" -Wpointer-arith -Wbad-function-cast -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings  -Wunreachable-code -Wstrict-aliasing=2 -Wwrite-strings -Wno-uninitialized #-Wno-aggressive-loop-optimizations -Wlogical-op
#-Wpadded  -Waggregate-return
#genpacket_LDADD = -lm libgp.la

src/cmdline.c src/cmdline.h: genpacket.ggo
	$(GENGETOPT) --output-dir=src/ < $<
    
src/cmdline_packit.c src/cmdline_packit.h: packit.ggo
	$(GENGETOPT) -f cmdline_packit_parser -a packit_args_info -F cmdline_packit --output-dir=src/ < $<


src/genpacket.tab.c: src/genpacket.y
	$(YACC) -d src/genpacket.y

src/genpacket.lex.c: src/genpacket.l
	$(LEX) src/genpacket.l

install-exec-hook:
	$(LN_S) $(DESTDIR)/bin/genpacket $(DESTDIR)/bin/packit

all-local:
	if [ ! -h $(top_srcdir)/src/packit ]; then \
		$(LN_S) $(shell pwd)/src/genpacket $(shell pwd)/src/packit; \
	fi

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
	$(GENERATE) -i $*.h_skel -F $*.h --output-format=c --separate-files --no-gen-version --force -f $$name
endif



#noinst_LTLIBRARIES = src/skels/libgp.la

#libgp_la_SOURCES = $(BUILT_SOURCES)

gengen_sources = \
	src/skels/calculated_packet_test_function_impl.c		\
	src/skels/calculated_packet_test_function_impl.h		\
	src/skels/cqc.c											\
	src/skels/cqc.h											\
	src/skels/dynamic_packet_test_function_impl.c			\
	src/skels/dynamic_packet_test_function_impl.h			\
	src/skels/fixed_packet_no_frame_test_function_impl.c	\
	src/skels/fixed_packet_no_frame_test_function_impl.h	\
	src/skels/fixed_packet_test_function_impl.c				\
	src/skels/fixed_packet_test_function_impl.h				\
	src/skels/header.c										\
	src/skels/header.h										\
	src/skels/receive_callback_decl.c						\
	src/skels/receive_callback_decl.h						\
	src/skels/send_func_impl.c								\
	src/skels/send_func_impl.h								\
	src/skels/send_function_decl.c							\
	src/skels/send_function_decl.h							\
	src/skels/source_skeleton.c								\
	src/skels/source_skeleton.h								\
	src/skels/structs.c										\
	src/skels/structs.h										\
	src/skels/var_declaration.c								\
	src/skels/var_declaration.h								\
	src/skels/write_data.c									\
	src/skels/write_data.h

EXTRA_DIST = \
	src/skels/calculated_packet_test_function_impl.h_skel		\
	src/skels/cqc.h_skel										\
	src/skels/dynamic_packet_test_function_impl.h_skel			\
	src/skels/fixed_packet_no_frame_test_function_impl.h_skel	\
	src/skels/fixed_packet_test_function_impl.h_skel			\
	src/skels/header.h_skel										\
	src/skels/receive_callback_decl.h_skel						\
	src/skels/send_func_impl.h_skel								\
	src/skels/send_function_decl.h_skel							\
	src/skels/source_skeleton.h_skel							\
	src/skels/structs.h_skel									\
	src/skels/var_declaration.h_skel							\
	src/skels/write_data.h_skel

#$(BUILT_SOURCES) $(TESTS)

TEST_EXTENSIONS = .test .ctest

built-clean:
	cd @srcdir@ && rm -f $(BUILT_SOURCES)

test/genpacket.c: src/genpacket
	./src/genpacket -i - -p test/fixed_basic.gp -o ./test/

TESTS = test/test_packit test/test_genpacket test/test_packit_output.test test/test_genpacket_output.test
check_PROGRAMS = test/test_packit test/test_genpacket test/genpacket_send
test_test_packit_SOURCES = test/test_packit.c $(genpacket_program_sources)
test_test_packit_CFLAGS = @check_CFLAGS@ -I src
test_test_packit_LDADD = @check_LIBS@
test_test_genpacket_SOURCES = test/genpacket.c test/test_genpacket.c
test_test_genpacket_CFLAGS = @check_CFLAGS@ -I test
test_test_genpacket_LDADD = @check_LIBS@
test_genpacket_send_SOURCES = test/genpacket.c test/genpacket_send.c
test_genpacket_send_CFLAGS =
test_genpacket_send_LDADD =

TEST_LOG_DRIVER = env AM_TAP_BATS='test/bats/bin/bats' $(SHELL) \
                  tap-driver.sh
                  
LOG_DRIVER =  env AM_TAP_AWK='$(AWK)' $(SHELL) \
                  tap-driver.sh


AM_TESTS_ENVIRONMENT = \
					PATH=test/bats/bin:$$PATH; export PATH; \
					if [ ! -d test/bats/bin ]; then \
						git clone git@github.com:sstephenson/bats.git test/bats;\
					fi;
