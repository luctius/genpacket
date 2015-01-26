TESTS = test_packit test_me.pl
check_PROGRAMS = test_packit
check_test_packit_SOURCES = test_packit.c $(top_builddir)/src/decoder.h
check_test_packit_CFLAGS = @CHECK_CFLAGS@
#check_money_LDADD = $(top_builddir)/src/libmoney.la @CHECK_LIBS@

TEST_EXTENSIONS = .pl
PL_LOG_COMPILER = $(PERL)
AM_PL_LOG_FLAGS = 
VERBOSE=1