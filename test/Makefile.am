TESTS = test_packit test_packit_output.test
check_PROGRAMS = test_packit
test_packit_SOURCES = test_packit.c $(top_builddir)/src/decoder.h
test_packit_CFLAGS = @CHECK_CFLAGS@

TEST_LOG_DRIVER = env AM_TAP_BATS='$(top_builddir)/test/bats/bin/bats' $(SHELL) \
                  $(top_srcdir)/tap-driver.sh

EXTRA_DIST = $(TESTS)

AM_TESTS_ENVIRONMENT = \
					PATH=$(top_builddir)/test/bats/bin:$$PATH; export PATH; \
					if [ ! -d $(top_builddir)/test/bats/bin ]; then \
						git clone git@github.com:sstephenson/bats.git; \
					fi;