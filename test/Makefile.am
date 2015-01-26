TESTS = test_packit test_packit_output.test
check_PROGRAMS = test_packit
test_packit_SOURCES = test_packit.c $(top_builddir)/src/decoder.h
test_packit_CFLAGS = @CHECK_CFLAGS@
#check_money_LDADD = $(top_builddir)/src/libmoney.la @CHECK_LIBS@

TEST_LOG_DRIVER = env AM_TAP_BATS='$(BATS)' $(SHELL) \
                  $(top_srcdir)/tap-driver.sh
				  
EXTRA_DIST = $(TESTS)