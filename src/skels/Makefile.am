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

.h_skel.c:
	name="`echo $* | sed 's/^.*\///g'`"; \
	echo "$$name"; \
	$(GENERATE) -i $< -F $*.h -f $$name --output-format=c --separate-files --output-dir=$(srcdir) --force
endif

INCLUDES = -I@top_srcdir@/src

noinst_LTLIBRARIES = libgp.la

libgp_la_SOURCES = $(BUILT_SOURCES)

BUILT_SOURCES = header.c header.h \
				structs.c structs.h \
				send_function.c send_function.h \
                receive_callback.c receive_callback.h \
				var_declaration.c var_declaration.h

EXTRA_DIST = header.h_skel \
			structs.h_skel \
            send_function.h_skel \
            receive_callback.h_skel \
			var_declaration.h_skel \
			$(BUILT_SOURCES)

built-clean:
	cd @srcdir@ && rm -f $(BUILT_SOURCES)
