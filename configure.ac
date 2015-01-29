AC_INIT(level_detector, 0.1.0)
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_LANG_C
AC_C_CONST
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_EXEEXT
AC_PROG_YACC
AM_PROG_LEX
LT_INIT
#AC_PROG_RANLIB

dnl the following is a program for generating a program that
dnl generates a source and substitutes some values
AC_PATH_PROGS(GENGEN, gengen)
AM_CONDITIONAL(NO_GENGEN, test -z "$ac_cv_path_GENGEN" )

AM_CONDITIONAL(NO_CROSSCOMPILING, test "$cross_compiling" = no )
AC_DEFINE([_GNU_SOURCE], 1, [Use GNU extensions])

PKG_CHECK_EXISTS([check], [PKG_CHECK_MODULES([check], check, [], [])], [AC_MSG_NOTICE([libCheck not found, tests will not be able to run]) ])

#AM_PATH_CHECK([0.9.0],[],[AC_MSG_ERROR([check is missing])])

# Checks for libraries.
AC_CHECK_LIB([m], [acos])

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([inttypes.h libintl.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h unistd.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_ERROR_AT_LINE
AC_FUNC_STRFTIME
AC_FUNC_STRTOD
AC_CHECK_FUNCS([memset strchr strdup strtol])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_HEADER_STDBOOL

AC_CONFIG_FILES([Makefile genpacket.ggo packit.ggo])
				 
AC_REQUIRE_AUX_FILE([tap-driver.sh])
AC_OUTPUT

